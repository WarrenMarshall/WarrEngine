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

struct w_index_buffer
{
	w_index_buffer();
	~w_index_buffer();

	unsigned int gl_id;

	virtual void bind();
	virtual void unbind();
};

struct w_index_buffer_quads : w_index_buffer
{
	w_index_buffer_quads();
};

// ----------------------------------------------------------------------------

struct w_render_batch
{
	e_render_prim render_prim = render_prim::quad;

	static int max_quads_per_batch;

	w_render_batch( e_render_prim render_prim );
	~w_render_batch();

	std::vector<w_batch_vert> vertices;

	GLuint VAO_id = 0;							// vertex array object
	GLuint VBO_id = 0;							// vertex buffer
	std::unique_ptr<w_index_buffer> index_buffer = nullptr;

	std::vector<int> texture_slots;
	int current_texture_slot_idx = 0;
	int assign_texture_slot( const a_texture* tex );

	void add_quad( const a_texture* tex, const w_batch_vert& v0, const w_batch_vert& v1, const w_batch_vert& v2, const w_batch_vert& v3 );
#if 0	// #batch
	void add_triangle( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2 );
	void add_line( const w_render_batch_vert& v0, const w_render_batch_vert& v1 );
#endif
	void bind();
	void unbind();
	void draw_and_reset();
	void reset();

private:
	void add_vert( const a_texture* tex, const w_batch_vert& render_vert );
};
