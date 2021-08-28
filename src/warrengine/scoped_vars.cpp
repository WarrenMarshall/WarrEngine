#include "master_pch.h"
#include "master_header.h"

namespace war
{

Scoped_Render_State::Scoped_Render_State()
{
	// duplicates the render_state at the top of the stack and makes it current
	g_engine->render.render_states.push_back( *Render::state );
	Render::state = &g_engine->render.render_states.back();
}

Scoped_Render_State::~Scoped_Render_State()
{
	assert( g_engine->render.render_states.size() > 1 );

	// pop the top state off the stack, and make the next one in line current
	g_engine->render.render_states.pop_back();
	Render::state = &g_engine->render.render_states.back();
}

// ----------------------------------------------------------------------------

Scoped_OpenGL::Scoped_OpenGL()
{
	g_engine->opengl_mgr.model_matrix_push();
}

Scoped_OpenGL::~Scoped_OpenGL()
{
	g_engine->opengl_mgr.model_matrix_pop();
}

// ----------------------------------------------------------------------------

Scoped_OpenGL_Identity::Scoped_OpenGL_Identity()
{
	g_engine->opengl_mgr.model_matrix_push_identity();
}

Scoped_OpenGL_Identity::~Scoped_OpenGL_Identity()
{
	g_engine->opengl_mgr.model_matrix_pop();
}

// ----------------------------------------------------------------------------

Scoped_UI_Layout::Scoped_UI_Layout( Rect rc )
{
	g_ui->layout_push( rc );
}

Scoped_UI_Layout::~Scoped_UI_Layout()
{
	g_ui->layout_pop();
}

}
