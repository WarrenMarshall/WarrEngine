
#pragma once

// ----------------------------------------------------------------------------

struct w_render_vertex
{
	w_render_vertex( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive );
	w_render_vertex( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive );

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

	unsigned int gl_id;
	std::vector<w_render_vertex> vertices;
	w_vertex_array_object* vertex_array_object = nullptr;

	// how many vertices make up a single element.
	// i.e. quad = 4, line = 2, point = 1
	int verts_per_element = -1;

	// #texture
	std::vector<const a_texture*> texture_slots;
	int current_texture_slot_idx = 0;

	virtual void bind();
	virtual void unbind();
	virtual void upload( int num_verts_to_upload );
	virtual void reset();

	void set_up_vertex_attribs();
	void preallocate_vertices( int max_verts );
	int assign_texture_slot( const a_texture* texture );
};
