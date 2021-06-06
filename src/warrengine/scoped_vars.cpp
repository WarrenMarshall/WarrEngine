#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

s_render_state::s_render_state()
{
	engine->render->push();
}

s_render_state::~s_render_state()
{
	engine->render->pop();
}

// ----------------------------------------------------------------------------

s_opengl::s_opengl()
{
	engine->opengl->push();
}

s_opengl::~s_opengl()
{
	engine->opengl->pop();
}

// ----------------------------------------------------------------------------

s_opengl_identity::s_opengl_identity()
{
	engine->opengl->push_identity();
}

s_opengl_identity::~s_opengl_identity()
{
	engine->opengl->pop();
}

// ----------------------------------------------------------------------------

s_imgui_location_offset::s_imgui_location_offset( const w_vec2& pivot )
	: pivot( pivot )
{
	engine->ui->imgui->add_location_offset( pivot );
}

s_imgui_location_offset::~s_imgui_location_offset()
{
	engine->ui->imgui->subtract_location_offset( pivot );
}

// ----------------------------------------------------------------------------

