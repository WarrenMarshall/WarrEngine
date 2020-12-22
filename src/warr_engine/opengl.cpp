#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

/*
	callback so OpenGL can relay messages and warnings
*/
void GLAPIENTRY OpenGL_MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
	if( type == GL_DEBUG_TYPE_ERROR )
	{
		log_error( "OpenGL Error: \"{}\"", message );
	}
}

// ----------------------------------------------------------------------------

void w_opengl::init()
{
	// Init GLEW
	GLenum err = glewInit();

	if( GLEW_OK != err )
		log_error( "GLEW failed to init : [{}]", glewGetErrorString( err ) );

	// sets a callback function so opengl can report errors and warnings to us
	glEnable( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( OpenGL_MessageCallback, nullptr );

	// opengl info
	log( "OpenGL Version : [{}]", glGetString( GL_VERSION ) );
	log( "GLEW Version : [{}]", glewGetString( GLEW_VERSION ) );
	log( "Renderer: [{}]", glGetString( GL_RENDERER ) );

	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units );
	log( "GL_MAX_TEXTURE_IMAGE_UNITS : {}", max_texture_image_units );

	GLint val;
	glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &val );
	log( "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS : {}", val );

	// #batch - this needs to be done in a cleaner way
	assert( max_texture_image_units >= 16 );

	// front facing triangles are wound counter clockwise
	glFrontFace( GL_CCW );
	glDisable( GL_CULL_FACE );

	// create base set of shaders
	base_shader_with_bright_pass = std::make_unique<w_shader>( "base.vert", "base_with_bright_pass.frag" );
	base_shader = std::make_unique<w_shader>( "base.vert", "base.frag" );
	blur_shader = std::make_unique<w_shader>( "base.vert", "blur.frag" );
	post_process_shader = std::make_unique<w_shader>( "base.vert", "post_process.frag" );

	for( int x = 0 ; x < max_texture_image_units ; ++x )
	{
		texture_slots.push_back( x );
	}
	set_uniform_array( "u_textures", texture_slots.data(), max_texture_image_units );

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
	// this is quick rejection only.
	//
	// (meaning that only pixels with an alpha value higher than 0.1 (or ~25) will get
	// through to the rest of the rendering pipeline)
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.1f );

	// default blending is transparency using the alpha channel
	glEnable( GL_BLEND );
	set_blend( opengl_blend::alpha );

	// smooth things look nicer
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );
}

// pushes a new matrix on top of the stack.
//
// this can either be an identity matrix, or a copy of the
// existing top matrix

w_matrix* w_opengl::push_identity()
{
	modelview_stack.emplace_back();
	return top();
}

w_matrix* w_opengl::push()
{
	modelview_stack.push_back( *top() );
	return top();
}

// removes the top matrix from the stack

w_matrix* w_opengl::pop()
{
	modelview_stack.pop_back();
	return top();
}

// returns a pointer to the top matrix on the stack

w_matrix* w_opengl::top()
{
	return &( modelview_stack.back() );
}

void w_opengl::set_blend( e_opengl_blend blend ) const
{
	switch( blend )
	{
		case opengl_blend::alpha:
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		break;

		case opengl_blend::add:
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		}
		break;

		case opengl_blend::multiply:
		{
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
		}
		break;
	}
}

// PROJECTION MATRIX - getting stuff into screen space from camera space

void w_opengl::init_projection_matrix() const
{
	RENDER->batch->flush();

	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::ortho<float>(
		0, v_window_w, v_window_h, 0,
		-20000.0f, 20000.0f );

	glProgramUniformMatrix4fv( base_shader->id, glGetUniformLocation( base_shader->id, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr( projection ) );
	glProgramUniformMatrix4fv( base_shader_with_bright_pass->id, glGetUniformLocation( base_shader_with_bright_pass->id, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr( projection ) );
	glProgramUniformMatrix4fv( blur_shader->id, glGetUniformLocation( blur_shader->id, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr( projection ) );
	glProgramUniformMatrix4fv( post_process_shader->id, glGetUniformLocation( post_process_shader->id, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr( projection ) );
}

// VIEW MATRIX - getting stuff into camera space from worldspace
//
// if there is a custom camera, apply it's transform.

void w_opengl::init_view_matrix( w_camera* camera ) const
{
	RENDER->batch->flush();

	// default to identity matrix
	glm::mat4 view = glm::mat4( 1.0f );

	if( camera )
	{
		view = glm::translate( view, glm::vec3(
			v_window_hw,
			v_window_hh,
			0.0f ) );

		view = glm::translate( view, glm::vec3(
			-( camera->pos.x ),
			-( camera->pos.y ),
			0.0f ) );
	}

	glProgramUniformMatrix4fv( base_shader->id, glGetUniformLocation( base_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( base_shader_with_bright_pass->id, glGetUniformLocation( base_shader_with_bright_pass->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( blur_shader->id, glGetUniformLocation( blur_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( post_process_shader->id, glGetUniformLocation( post_process_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
}

// VIEW MATRIX - getting stuff into camera space from worldspace
//
// This sets the view matrix as identity for things like UI and mouse cursors.

void w_opengl::init_view_matrix_identity() const
{
	RENDER->batch->flush();

	glm::mat4 view = glm::mat4( 1.0f );

	glProgramUniformMatrix4fv( base_shader->id, glGetUniformLocation( base_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( base_shader_with_bright_pass->id, glGetUniformLocation( base_shader_with_bright_pass->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( blur_shader->id, glGetUniformLocation( blur_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( post_process_shader->id, glGetUniformLocation( post_process_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
}

void w_opengl::init_view_matrix_identity_ui() const
{
	RENDER->batch->flush();

	glm::mat4 view = glm::mat4( 1.0f );
	view *= glm::scale( view, glm::vec3( ui_canvas_scale, ui_canvas_scale, 1.0f ) );

	glProgramUniformMatrix4fv( base_shader->id, glGetUniformLocation( base_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( base_shader_with_bright_pass->id, glGetUniformLocation( base_shader_with_bright_pass->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( blur_shader->id, glGetUniformLocation( blur_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
	glProgramUniformMatrix4fv( post_process_shader->id, glGetUniformLocation( post_process_shader->id, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr( view ) );
}

void w_opengl::set_uniform( std::string_view name, float value )
{
	glProgramUniform1f( base_shader->id, glGetUniformLocation( base_shader->id, name.data() ), value );
	glProgramUniform1f( base_shader_with_bright_pass->id, glGetUniformLocation( base_shader_with_bright_pass->id, name.data() ), value );
	glProgramUniform1f( blur_shader->id, glGetUniformLocation( blur_shader->id, name.data() ), value );
	glProgramUniform1f( post_process_shader->id, glGetUniformLocation( post_process_shader->id, name.data() ), value );
}

void w_opengl::set_uniform( std::string_view name, bool value )
{
	glProgramUniform1i( base_shader->id, glGetUniformLocation( base_shader->id, name.data() ), value );
	glProgramUniform1i( base_shader_with_bright_pass->id, glGetUniformLocation( base_shader_with_bright_pass->id, name.data() ), value );
	glProgramUniform1i( blur_shader->id, glGetUniformLocation( blur_shader->id, name.data() ), value );
	glProgramUniform1i( post_process_shader->id, glGetUniformLocation( post_process_shader->id, name.data() ), value );
}

void w_opengl::set_uniform( std::string_view name, w_color value )
{
	glProgramUniform4f( base_shader->id, glGetUniformLocation( base_shader->id, name.data() ), value.r, value.g, value.b, value.a );
	glProgramUniform4f( base_shader_with_bright_pass->id, glGetUniformLocation( base_shader_with_bright_pass->id, name.data() ), value.r, value.g, value.b, value.a );
	glProgramUniform4f( blur_shader->id, glGetUniformLocation( blur_shader->id, name.data() ), value.r, value.g, value.b, value.a );
	glProgramUniform4f( post_process_shader->id, glGetUniformLocation( post_process_shader->id, name.data() ), value.r, value.g, value.b, value.a );
}

void w_opengl::set_uniform_array( std::string_view name, int* value, int count )
{
	GLint loc;

	loc = glGetUniformLocation( base_shader->id, name.data() );
	glProgramUniform1iv(
		base_shader->id,
		loc,
		count,
		value );

	loc = glGetUniformLocation( base_shader_with_bright_pass->id, name.data() );
	glProgramUniform1iv(
		base_shader_with_bright_pass->id,
		loc,
		count,
		value );

	loc = glGetUniformLocation( blur_shader->id, name.data() );
	glProgramUniform1iv(
		blur_shader->id,
		loc,
		count,
		value );

	loc = glGetUniformLocation( post_process_shader->id, name.data() );
	glProgramUniform1iv(
		post_process_shader->id,
		loc,
		count,
		value );
}
