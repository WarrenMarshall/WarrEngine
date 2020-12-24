
#pragma once

// ----------------------------------------------------------------------------

struct w_vertex_buffer
{
	w_vertex_buffer();
	~w_vertex_buffer();

	unsigned int gl_id;
	std::vector<w_render_vertex> vertices;

	virtual void bind();
	virtual void unbind();
	virtual void upload( int num_verts_to_upload ) {}
	virtual void reset();

	void set_up_vertex_attribs();
	void preallocate_vertices( int max_verts );
};

// ----------------------------------------------------------------------------

struct w_vertex_buffer_quads : w_vertex_buffer
{
	w_vertex_buffer_quads();

	virtual void upload( int num_verts_to_upload ) override;
};
