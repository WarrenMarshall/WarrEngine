#pragma once

// ----------------------------------------------------------------------------

struct w_batch_vert
{
	w_batch_vert( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive );
	w_batch_vert( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive );

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

	w_render_batch();
	~w_render_batch();

	std::vector<w_batch_vert> vertices;

	GLuint VAO_id = 0;							// vertex array object
	GLuint VBO_id = 0;							// vertex buffer
	GLuint EBO_id = 0;							// index buffer

	std::vector<int> texture_slots;
	a_texture* current_texture = nullptr;
	int current_texture_slot_idx = 0;
	void set_current_texture( a_texture* tex );

	void add_quad( const w_batch_vert& v0, const w_batch_vert& v1, const w_batch_vert& v2, const w_batch_vert& v3 );
#if 0	// #batch
	void add_triangle( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2 );
	void add_line( const w_render_batch_vert& v0, const w_render_batch_vert& v1 );
#endif
	void bind();
	void unbind();
	void draw_and_reset();
	void reset();

	void add_vert( const w_batch_vert& render_vert );
};
