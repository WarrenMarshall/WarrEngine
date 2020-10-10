#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

/*
	callback so OpenGL can relay messages and warnings
*/
void GLAPIENTRY OpenGL_MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    if( type == GL_DEBUG_TYPE_ERROR )
        log_error( "OpenGL Error: [{}]", message );
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
	glDebugMessageCallback( OpenGL_MessageCallback, 0 );

	// opengl info
	log_msg( "OpenGL Version : [{}]", glGetString( GL_VERSION ) );
	log_msg( "GLEW Version : [{}]", glewGetString( GLEW_VERSION ) );
	log_msg( "Renderer: [{}]", glGetString( GL_RENDERER ) );

	// front facing triangles are wound counter clockwise
	glFrontFace( GL_CCW );
	glDisable( GL_CULL_FACE );

	engine->shader_ui->create_and_compile( "simple", "simple" );
	engine->shader_crt->create_and_compile( "simple", "crt" );
	engine->shader_to_screen->create_and_compile( "simple", "simple" );

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
	modelview_stack.push_back( w_matrix() );
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
	return &(modelview_stack.back());
}

void w_opengl::clear_texture_bind() const
{
	glBindTexture( GL_TEXTURE_2D, 0 );
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
