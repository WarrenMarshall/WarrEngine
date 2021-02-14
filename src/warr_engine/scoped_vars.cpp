#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

render_push_pop::render_push_pop()
{
	RENDER->push();
}

render_push_pop::~render_push_pop()
{
	RENDER->pop();
}

// ----------------------------------------------------------------------------

opengl_push_pop::opengl_push_pop()
{
	OPENGL->push();
}

opengl_push_pop::~opengl_push_pop()
{
	OPENGL->pop();
}

// ----------------------------------------------------------------------------

opengl_identity_push_pop::opengl_identity_push_pop()
{
	OPENGL->push_identity();
}

opengl_identity_push_pop::~opengl_identity_push_pop()
{
	OPENGL->pop();
}

// ----------------------------------------------------------------------------
