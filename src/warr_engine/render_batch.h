#pragma once

// ----------------------------------------------------------------------------

struct w_render_batch_vert
{
	w_render_batch_vert( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive );
	w_render_batch_vert( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive );

	float x, y, z;
	float u, v;
	float r, g, b, a;
	float e;
	float t;
};

// ----------------------------------------------------------------------------

struct w_render_batch
{
	static int max_quads_per_batch;
	static int max_texture_units;

	w_render_batch();
	~w_render_batch();

	std::vector<w_render_batch_vert> vertices;
	int num_quads_to_render = 0;
	GLuint VAO = 0;							// vertex array object
	GLuint VBO = 0;							// vertex buffer
	GLuint EBO = 0;							// index buffer

	int texture_slots[ 32 ];
	int texture_slot_idx = 0;
	int add_texture_slot( a_texture* tex );

	void add_quad( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2, const w_render_batch_vert& v3 );
#if 0	// #batch
	void add_triangle( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2 );
	void add_line( const w_render_batch_vert& v0, const w_render_batch_vert& v1 );
#endif
	void bind();
	void unbind();
	void flush();
	void reset();

	void add_render_vert( const w_render_batch_vert& render_vert );
};
