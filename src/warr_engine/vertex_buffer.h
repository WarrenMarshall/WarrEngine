
#pragma once

// ----------------------------------------------------------------------------

struct w_render_vertex
{
	w_render_vertex( const w_vec2& pos, const w_uv& uv, const w_color& color, const float glow );
	w_render_vertex( const w_vec3& pos, const w_uv& uv, const w_color& color, const float glow );

	float x, y, z;
	float u, v;
	float r, g, b, a;
	float e;
	float t;
};

// ----------------------------------------------------------------------------

struct w_vertex_buffer
{
	w_vertex_buffer( w_vertex_array_object* vertex_array_object, int verts_per_element );
	~w_vertex_buffer();

	w_vertex_array_object* vertex_array_object = nullptr;
	unsigned int gl_id;

	// pool of contiguous vertex data
	std::vector<w_render_vertex> vertices;

	// how many vertices make up a single element.
	// i.e. quad = 4, triangles = 3, line = 2, point = 1
	int verts_per_element = -1;

	std::vector<const a_texture*> texture_slots;
	int total_texture_slots_used = 0;

	virtual void bind();
	virtual void unbind();
	void bind_texture_units();
	virtual void upload();
	virtual void reset();

	void set_up_vertex_attribs();
	void preallocate_vertices( int max_verts );
	int assign_texture_slot( const a_texture* texture );
};
