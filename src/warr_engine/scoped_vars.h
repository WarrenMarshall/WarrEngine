#pragma once

// ----------------------------------------------------------------------------

struct s_render_state
{
	s_render_state();
	~s_render_state();
};

#define scoped_render_state	scoped_decl( s_render_state, __LINE__)

// ----------------------------------------------------------------------------

struct s_opengl
{
	s_opengl();
	~s_opengl();
};

#define scoped_opengl	scoped_decl( s_opengl, __LINE__)

// ----------------------------------------------------------------------------

struct s_opengl_identity
{
	s_opengl_identity();
	~s_opengl_identity();
};

#define scoped_opengl_identity	scoped_decl( s_opengl_identity, __LINE__)

// ----------------------------------------------------------------------------

struct s_imgui_location_offset
{
	w_vec2 pivot;

	s_imgui_location_offset() = default;
	s_imgui_location_offset( const w_vec2& pivot );
	~s_imgui_location_offset();
};

#define scoped_imgui_location_offset(arg1)	scoped_decl_arg1( s_imgui_location_offset, arg1, __LINE__)

// ----------------------------------------------------------------------------
