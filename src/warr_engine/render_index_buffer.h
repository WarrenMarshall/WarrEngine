#pragma once

// ----------------------------------------------------------------------------

struct w_index_buffer
{
	w_index_buffer( w_render_batch* batch );
	~w_index_buffer();

	w_render_batch* batch = nullptr;
	unsigned int gl_id;

	virtual void bind();
	virtual void unbind();
};

// ----------------------------------------------------------------------------

struct w_index_buffer_quads : w_index_buffer
{
	w_index_buffer_quads( w_render_batch* batch );
};

// ----------------------------------------------------------------------------

struct w_index_buffer_tris : w_index_buffer
{
	w_index_buffer_tris( w_render_batch* batch );
};

// ----------------------------------------------------------------------------

struct w_index_buffer_lines : w_index_buffer
{
	w_index_buffer_lines( w_render_batch* batch );
};

