
#pragma once

// ----------------------------------------------------------------------------

struct w_vertex_buffer
{
	w_vertex_buffer( w_render_batch* batch, int verts_per_element );
	~w_vertex_buffer();

	w_render_batch* batch = nullptr;
	unsigned int gl_id;
	std::vector<w_render_vertex> vertices;

	// how many vertices make up a single element.
	// i.e. quad = 4, line = 2, point = 1
	int verts_per_element = -1;

	virtual void bind();
	virtual void unbind();
	virtual void upload( int num_verts_to_upload );
	virtual void reset();

	void set_up_vertex_attribs();
	void preallocate_vertices( int max_verts );
};
