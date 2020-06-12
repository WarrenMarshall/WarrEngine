#include "master_pch.h"
#include "master_header.h"

/*
	callback so OpenGL can relay messages and warnings
*/
void GLAPIENTRY OpenGL_MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    if( type == GL_DEBUG_TYPE_ERROR )
        log_error( "OpenGL Error: [%s]", message );
	else
	{
		// this is VERY spammy so use for debugging purposes only
        //log_msg( "OpenGL : [%s]", message );
	}
}

void w_opengl::init()
{
	// Init GLEW
	GLenum err = glewInit();

	if( GLEW_OK != err )
		log_error( "%s : GLEW failed to init : [%s]", __FUNCTION__, glewGetErrorString( err ) );

	// sets a callback function so opengl can report errors and warnings to us
	glEnable( GL_DEBUG_OUTPUT );
	glDebugMessageCallback( OpenGL_MessageCallback, 0 );

	// opengl info
	log_msg( "OpenGL Version : [%s]", glGetString( GL_VERSION ) );
	log_msg( "GLEW Version : [%s]", glewGetString( GLEW_VERSION ) );
	log_msg( "Renderer: [%s]", glGetString( GL_RENDERER ) );

	// front facing polygons are wound clockwise
	glFrontFace( GL_CW );

#ifdef _DEBUG
	// in debug mode, use a shader that colors backfacing triangles in red and disables culling
	glDisable( GL_CULL_FACE );
	engine->shader->create_and_compile( "simple", "simple_debug" );
#else
	// in release mode, use a more optimal shader and turn on backface culling
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	engine->shader->create_and_compile( "simple", "simple" );
#endif

	// texture mapping
	glEnable( GL_TEXTURE_2D );

	// z/depth buffer testing
	glEnable( GL_DEPTH_TEST );	// turn on z-buffer
	glDepthFunc( GL_LEQUAL );
	glDepthMask( GL_TRUE );		// enable depth writing (reading is always enabled)
								// when we get fancier, we could toggle writing off for things that are transparent (and render those things last each frame)

	// allows the alpha channel of the texture to be used for transparency - this has nothing
	// to go with drawing things transparently or blending.
	//
	// this is strictly to let alpha channels in textures reject pixels for both drawing
	// and writing to the depth buffer.
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, 0.10f );

	// by default, set up blending so that alpha channels will work
	glEnable(GL_BLEND);
	set_blend( e_opengl_blend::alpha );

	// smooth things look nicer
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POINT_SMOOTH );

	//glLineWidth( 4.0f );
}

void w_opengl::deinit()
{

}

void w_opengl::push_matrix()
{
	glm::mat4 mtx = *top_matrix();
	modelview_stack.push( mtx );
}

void w_opengl::push_identity_matrix()
{
	modelview_stack.push( glm::mat4( 1 ) );
}

void w_opengl::pop_matrix()
{
	modelview_stack.pop();
}

glm::mat4* w_opengl::top_matrix()
{
	return &(modelview_stack.top());
}

void w_opengl::add_transform( const i_transform& t )
{
	translate( t.pos );
	rotate( t.angle );
	scale( t.scale );
}

void w_opengl::translate( w_vec3 v )
{
	if( !v.is_zero() )
	{
		glm::mat4* mptr = top_matrix();
		*mptr = glm::translate( *mptr, glm::vec3( v.x, v.y, v.z ) );
	}
}

void w_opengl::scale( float v )
{
	if( !fequals( v, 1.0f ) )
	{
		glm::mat4* mptr = top_matrix();
		*mptr = glm::scale( *mptr, glm::vec3( v, v, v ) );
	}
}

void w_opengl::rotate( float v )
{
	if( fmod( v, 360.0f ) )
	{
		glm::mat4* mptr = top_matrix();
		*mptr = glm::rotate( *mptr, glm::radians( v ), glm::vec3( 0.f, 0.f, 1.f ) );
	}
}

void w_opengl::clear_texture_bind()
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void w_opengl::set_blend( e_opengl_blend blend )
{
	switch( blend )
	{
		case e_opengl_blend::alpha:
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		break;

		case e_opengl_blend::add:
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		}
		break;

		case e_opengl_blend::multiply:
		{
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
		}
		break;
	}
}

// updates the size of opengl primitives based on the ratio
// between the window size and the virutual screen size
//
// this makes it so lines and points look more like they
// belong in the game world.

void w_opengl::refresh_primitive_sizes()
{
	int w, h;
	glfwGetWindowSize( engine->window->window, &w, &h );

	float w_ratio = w / v_window_w;
	float h_ratio = h / v_window_h;

	if( w_ratio > h_ratio )
	{
		glLineWidth( w_ratio );
		glPointSize( w_ratio );
	}
	else
	{
		glLineWidth( h_ratio );
		glPointSize( h_ratio );
	}
}