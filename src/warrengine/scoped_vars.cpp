#include "master_pch.h"
#include "master_header.h"

namespace war
{

s_render_state::s_render_state()
{
	// duplicates the render_state at the top of the stack and makes it current
	g_engine->renderer.render_states.push_back( *render::state );
	render::state = &g_engine->renderer.render_states.back();
}

s_render_state::~s_render_state()
{
	assert( g_engine->renderer.render_states.size() > 1 );

	// pop the top state off the stack, and make the next one in line current
	g_engine->renderer.render_states.pop_back();
	render::state = &g_engine->renderer.render_states.back();
}

// ----------------------------------------------------------------------------

s_opengl::s_opengl()
{
	g_engine->render_api.model_matrix_push();
}

s_opengl::~s_opengl()
{
	g_engine->render_api.model_matrix_pop();
}

// ----------------------------------------------------------------------------

s_opengl_identity::s_opengl_identity()
{
	g_engine->render_api.model_matrix_push_identity();
}

s_opengl_identity::~s_opengl_identity()
{
	g_engine->render_api.model_matrix_pop();
}

// ----------------------------------------------------------------------------

s_ui_layout::s_ui_layout( rect rc )
{
	g_ui->layout_push( rc );
}

s_ui_layout::~s_ui_layout()
{
	g_ui->layout_pop();
}

}
