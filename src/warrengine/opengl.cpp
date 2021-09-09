
#include "master_pch.h"
#include "master_header.h"

namespace war
{

#include "opengl_error_handling.cpp"

// ----------------------------------------------------------------------------

void OpenGL_Mgr::init()
{
	// Init GLEW
	GLenum err = glewInit();

	if( GLEW_OK != err )
	{
		log_fatal( "GLEW failed to init : [{}]", (char*)glewGetErrorString( err ) );
	}

	// sets a callback function so opengl can report errors and warnings to us
	glEnable( GL_DEBUG_OUTPUT );
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
	glDebugMessageCallback( OpenGL_MessageCallback, nullptr );

	// opengl info
	log( "OpenGL Version : [{}]", (char*)glGetString( GL_VERSION ) );
	log( "GLEW Version : [{}]", (char*)glewGetString( GLEW_VERSION ) );
	log( "Renderer: [{}]", (char*)glGetString( GL_RENDERER ) );

	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units );

	if( g_engine->cmdline.nobatch )
	{
		max_texture_image_units = 1;
	}

	log( "GL_MAX_TEXTURE_IMAGE_UNITS : {}", max_texture_image_units );

	// how many color attachments can be added to a frame buffer object
	int32_t max_color_attachments = 0;
	glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments );
	log( "GL_MAX_COLOR_ATTACHMENTS : {} (per frame buffer object)", max_color_attachments );

	// the maximum number of buffers that can be written to at once from a fragment shader
	int32_t max_draw_buffers = 0;
	glGetIntegerv( GL_MAX_DRAW_BUFFERS, &max_draw_buffers );
	log( "GL_MAX_DRAW_BUFFERS : {}", max_draw_buffers );

	// front facing triangles are wound counter clockwise
	glFrontFace( GL_CCW );

	// some sprites like to use flipping and negative scaling, so backface culling needs to be disabled
	glDisable( GL_CULL_FACE );

	// enable cheapo stencil buffer
	//glEnable( GL_SCISSOR_TEST );

	// create the shaders we need

	shaders[ "simple" ] = Shader( "simple.vert", "simple.frag" );
	shaders[ "base_pass" ] = Shader( "simple.vert", "base_pass.frag" );
	shaders[ "blur_box" ] = Shader( "simple.vert", "blur_box.frag" );
	shaders[ "blur_gaussian" ] = Shader( "simple.vert", "blur_gaussian.frag" );
	shaders[ "compositing_pass" ] = Shader( "simple.vert", "compositing_pass.frag" );
	shaders[ "final_pass" ] = Shader( "simple.vert", "final_pass.frag" );

	// we only have to do this one time, so a local array is created, filled,
	// uploaded, and left for dead

	std::vector<int> texture_slots( max_texture_image_units );
	for( auto x = 0 ; x < max_texture_image_units ; ++x )
	{
		texture_slots[ x ] = x;
	}
	set_uniform_array( "u_textures", texture_slots.data(), max_texture_image_units );

	// set up opengl states

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glEnable( GL_TEXTURE_2D );

	// NOTE : we have to use a depth buffer because we are rendering in batches.
	//		  it's tempting to try and remove this and think of the renderer as a
	//		  back-to-front series of quad draws, but that's not how it works.
	//
	//		  disabling the depth buffer is a trap that will cost you time until you
	//		  remember this. just walk away.

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	// allows the alpha channel of the texture to be used for masking  - this has nothing
	// to do with drawing things with transparency or blending.
	//
	// this is quick rejection only, at the texture alpha channel level.
	//
	// (meaning that only pixels with an alpha value higher than 0.1 (or ~25) will get
	// through to the rest of the rendering pipeline)
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.1f );

	// default blending is transparency using the alpha channel
	glEnable( GL_BLEND );
	set_blend( e_opengl_blend::alpha );

	// smooth things look nicer
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );

	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );

	glLineWidth( 1.f );
	glPointSize( 2.f );

	glClampColor( GL_CLAMP_READ_COLOR, GL_FALSE );
	glClampColor( GL_CLAMP_VERTEX_COLOR, GL_FALSE );
	glClampColor( GL_CLAMP_FRAGMENT_COLOR, GL_FALSE );

	// index buffers that everyone can use since the formatting and internals of
	// them never change. all rendering code references these and they are only
	// uploaded to the GPU one time.

	ib_quads = std::make_unique<Index_Buffer_Quads>();
	ib_tris = std::make_unique<Index_Buffer_Tris>();
	ib_lines = std::make_unique<Index_Buffer_Lines>();
	ib_points = std::make_unique<Index_Buffer_Points>();
}

// pushes a new matrix on top of the stack.
//
// this can either be an identity matrix, or a copy of the
// existing top matrix

Matrix* OpenGL_Mgr::model_matrix_push_identity()
{
	model_matrix_stack.emplace_back();
	top_matrix = &( model_matrix_stack.back() );
	return top_matrix;
}

Matrix* OpenGL_Mgr::model_matrix_push()
{
	model_matrix_stack.push_back( model_matrix_stack.back() );
	top_matrix = &( model_matrix_stack.back() );
	return top_matrix;
}

// removes the top matrix from the stack

Matrix* OpenGL_Mgr::model_matrix_pop()
{
	model_matrix_stack.pop_back();
	top_matrix = &( model_matrix_stack.back() );
	return top_matrix;
}

void OpenGL_Mgr::clear_depth_buffer()
{
	glClear( GL_DEPTH_BUFFER_BIT );
}

void OpenGL_Mgr::set_blend( e_opengl_blend_t blend ) const
{
	switch( blend )
	{
		case e_opengl_blend::alpha:
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			break;
		}

		case e_opengl_blend::add:
		{
			glBlendFunc( GL_ONE, GL_ONE );
			break;
		}

		case e_opengl_blend::multiply:
		{
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			break;
		}

		case e_opengl_blend::glow:
		{
			glBlendFunc( GL_SRC_COLOR, GL_ONE );
			break;
		}
	}
}

// PROJECTION MATRIX - getting stuff into screen space from camera space

void OpenGL_Mgr::set_projection_matrix() const
{
	glm::mat4 projection = glm::mat4( 1.f );

	projection = glm::ortho<float_t>(
		0.f,
		viewport_w,
		viewport_h,
		0.f,
		-zdepth_max,
		zdepth_max );

	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		glProgramUniformMatrix4fv(
			Shader.gl_id,
			glGetUniformLocation( Shader.gl_id, "u_projection_matrix" ),
			1,
			GL_FALSE,
			glm::value_ptr( projection )
		);
	}
}

// sets up the default view matrix

void OpenGL_Mgr::set_view_matrix_identity()
{
	//assert( g_engine->renderer.dynamic_batches.is_empty() );

	// identity

	Matrix view_mtx;

	// viewport pivot

	Matrix viewport_pivot_mtx;
	viewport_pivot_mtx.translate( g_engine->scene_mgr.get_viewport_pivot() );

	// camera

	auto cam_mtx = g_engine->scene_mgr.get_transform()->to_matrix();
	view_mtx *= cam_mtx;
	using_camera = true;

	// send to all shaders

	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		glProgramUniformMatrix4fv(
			Shader.gl_id,
			glGetUniformLocation( Shader.gl_id, "u_view_matrix" ),
			1,
			GL_FALSE,
			glm::value_ptr( view_mtx.m )
		);
		glProgramUniformMatrix4fv(
			Shader.gl_id,
			glGetUniformLocation( Shader.gl_id, "u_viewport_pivot_matrix" ),
			1,
			GL_FALSE,
			glm::value_ptr( viewport_pivot_mtx.m )
		);
	}
}

void OpenGL_Mgr::set_view_matrix_identity_no_camera()
{
	//assert( g_engine->renderer.dynamic_batches.is_empty() );

	// identity

	Matrix view_mtx;
	Matrix viewport_pivot_mtx;

	using_camera = false;

	// send to all shaders

	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		glProgramUniformMatrix4fv(
			Shader.gl_id,
			glGetUniformLocation( Shader.gl_id, "u_view_matrix" ),
			1,
			GL_FALSE,
			glm::value_ptr( view_mtx.m )
		);
		glProgramUniformMatrix4fv(
			Shader.gl_id,
			glGetUniformLocation( Shader.gl_id, "u_viewport_pivot_matrix" ),
			1,
			GL_FALSE,
			glm::value_ptr( viewport_pivot_mtx.m )
		);
	}
}

// sets the view matrix as identity for things like UI and mouse cursors.

void OpenGL_Mgr::set_view_matrix_identity_ui()
{
	//assert( g_engine->renderer.dynamic_batches.is_empty() );

	// identity

	Matrix view_mtx;
	Matrix viewport_pivot_mtx;

	using_camera = false;

	// ui scale

	view_mtx.scale( ui_scale );

	// send to all shaders

	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		glProgramUniformMatrix4fv(
			Shader.gl_id,
			glGetUniformLocation( Shader.gl_id, "u_view_matrix" ),
			1,
			GL_FALSE,
			glm::value_ptr( view_mtx.m )
		);
		glProgramUniformMatrix4fv(
			Shader.gl_id,
			glGetUniformLocation( Shader.gl_id, "u_viewport_pivot_matrix" ),
			1,
			GL_FALSE,
			glm::value_ptr( viewport_pivot_mtx.m )
		);
	}
}

// the "get_uniform_" functions loop through all shaders and attempt to locate
// the requested uniform name. they will read and return the first one they
// find.

float_t OpenGL_Mgr::get_uniform_float( std::string_view name )
{
	float_t result = 0.f;

	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		auto loc = glGetUniformLocation( Shader.gl_id, name.data() );

		if( loc != -1 )
		{
			glGetUniformfv( Shader.gl_id, loc, &result );
			break;
		}
	}

	return result;
}

bool OpenGL_Mgr::get_uniform_bool( std::string_view name )
{
	int32_t result = 0;

	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		auto loc = glGetUniformLocation( Shader.gl_id, name.data() );

		if( loc != -1 )
		{
			glGetUniformiv( Shader.gl_id, loc, &result );
			break;
		}
	}

	return bool( result );
}

Color OpenGL_Mgr::get_uniform_color( std::string_view name )
{
	Color result;

	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		auto loc = glGetUniformLocation( Shader.gl_id, name.data() );

		if( loc != -1 )
		{
			glGetnUniformfv( Shader.gl_id, loc, 4 * sizeof( float_t ), &result.r );
			break;
		}
	}

	return result;
}

// the "set_uniform" functions loop through all shaders and attempt to set a
// uniform value in each one, if it exists.

void OpenGL_Mgr::set_uniform_float( std::string_view name, float_t value )
{
	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		auto loc = glGetUniformLocation( Shader.gl_id, name.data() );
		if( loc != -1 )
		{
			glProgramUniform1f( Shader.gl_id, loc, value );
		}
	}
}

void OpenGL_Mgr::set_uniform_bool( std::string_view name, bool value )
{
	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		auto loc = glGetUniformLocation( Shader.gl_id, name.data() );
		if( loc != -1 )
		{
			glProgramUniform1i( Shader.gl_id, loc, value );
		}
	}
}

void OpenGL_Mgr::set_uniform_color( std::string_view name, const Color& value )
{
	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		auto loc = glGetUniformLocation( Shader.gl_id, name.data() );
		if( loc != -1 )
		{
			glProgramUniform4f( Shader.gl_id, loc, value.r, value.g, value.b, value.a );
		}
	}
}

void OpenGL_Mgr::set_uniform_array( std::string_view name, int32_t* value, int32_t count )
{
	for( auto& [shader_name, Shader] : g_engine->opengl_mgr.shaders )
	{
		auto loc = glGetUniformLocation( Shader.gl_id, name.data() );
		if( loc != -1 )
		{
			glProgramUniform1iv( Shader.gl_id, loc, count, value );
		}
	}
}

// pre-allocate memory on the card for all the verts we will ever use
//
// dynamic buffers allocate the maximum number allowed, while static buffers
// will allocate exactly what they need.
//
// note : buffer must already be bound or this will crash

void OpenGL_Mgr::allocate_vertex_buffer_on_gpu( int32_t max_verts, bool is_static )
{
	glBufferData(
		GL_ARRAY_BUFFER,
		max_verts * sizeof( Render_Vertex ),
		nullptr,
		is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
	);
}

}
