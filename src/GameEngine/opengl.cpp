#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

void w_opengl::init() const
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
}

// pushes a new matrix on top of the stack.
//
// this can either be an identity matrix, or a copy of the
// existing top matrix

w_matrix* w_opengl::push_identity()
{
	modelview_stack.push( w_matrix() );
	return top();
}

w_matrix* w_opengl::push()
{
	modelview_stack.push( *top() );
	return top();
}

// removes the top matrix from the stack

w_matrix* w_opengl::pop()
{
	modelview_stack.pop();
	return top();
}

// returns a pointer to the top matrix on the stack

w_matrix* w_opengl::top()
{
	return &(modelview_stack.top());
}

void w_opengl::clear_texture_bind() const
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void w_opengl::set_blend( e_opengl_blend blend ) const
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

void w_opengl::refresh_primitive_sizes() const
{
	int w, h;
	glfwGetWindowSize( engine->window->window, &w, &h );

	// this can happen if the user does something like WIN+D.
	// this check prevents an opengl crash below when setting line size.
	if( !w || !h )
	{
		return;
	}

	const float w_ratio = w / v_window_w;
	const float h_ratio = h / v_window_h;

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
