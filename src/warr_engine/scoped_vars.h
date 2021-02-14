#pragma once

// ----------------------------------------------------------------------------

struct render_push_pop
{
	render_push_pop();
	~render_push_pop();
};

#define scoped_render_push_pop	scoped_decl( render_push_pop, __LINE__)

// ----------------------------------------------------------------------------

struct opengl_push_pop
{
	opengl_push_pop();
	~opengl_push_pop();
};

#define scoped_opengl_push_pop	scoped_decl( opengl_push_pop, __LINE__)

// ----------------------------------------------------------------------------

struct opengl_identity_push_pop
{
	opengl_identity_push_pop();
	~opengl_identity_push_pop();
};

#define scoped_opengl_identity_push_pop	scoped_decl( opengl_identity_push_pop, __LINE__)

// ----------------------------------------------------------------------------
