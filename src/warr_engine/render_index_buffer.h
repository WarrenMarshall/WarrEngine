#pragma once

// ----------------------------------------------------------------------------

struct w_index_buffer
{
	w_index_buffer( w_vertex_array_object* vertex_array_object );
	~w_index_buffer();

	w_vertex_array_object* vertex_array_object = nullptr;

	unsigned int gl_id;

	virtual void bind();
	virtual void unbind();
};

// ----------------------------------------------------------------------------

struct w_index_buffer_quads : w_index_buffer
{
	w_index_buffer_quads( w_vertex_array_object* vertex_array_object );
};

// ----------------------------------------------------------------------------

struct w_index_buffer_tris : w_index_buffer
{
	w_index_buffer_tris( w_vertex_array_object* vertex_array_object );
};

// ----------------------------------------------------------------------------

struct w_index_buffer_lines : w_index_buffer
{
	w_index_buffer_lines( w_vertex_array_object* vertex_array_object );
};

// ----------------------------------------------------------------------------

struct w_index_buffer_points : w_index_buffer
{
	w_index_buffer_points( w_vertex_array_object* vertex_array_object );
};

