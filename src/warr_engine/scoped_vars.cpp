#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

s_render_state::s_render_state()
{
	RENDER->push();
}

s_render_state::~s_render_state()
{
	RENDER->pop();
}

// ----------------------------------------------------------------------------

s_opengl::s_opengl()
{
	OPENGL->push();
}

s_opengl::~s_opengl()
{
	OPENGL->pop();
}

// ----------------------------------------------------------------------------

s_opengl_identity::s_opengl_identity()
{
	OPENGL->push_identity();
}

s_opengl_identity::~s_opengl_identity()
{
	OPENGL->pop();
}

// ----------------------------------------------------------------------------

s_imgui_location_offset::s_imgui_location_offset( const w_vec2& pivot )
	: pivot( pivot )
{
	IMGUI->add_location_offset( pivot );
}

s_imgui_location_offset::~s_imgui_location_offset()
{
	IMGUI->subtract_location_offset( pivot );
}

// ----------------------------------------------------------------------------

