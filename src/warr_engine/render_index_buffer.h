#pragma once

// ----------------------------------------------------------------------------

struct w_index_buffer
{
	w_index_buffer();
	~w_index_buffer();

	unsigned int gl_id;

	virtual void bind();
	virtual void unbind();
};

// ----------------------------------------------------------------------------

struct w_index_buffer_quads : w_index_buffer
{
	w_index_buffer_quads();
};

