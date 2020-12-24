#pragma once

struct w_render_batch
{
	e_render_prim render_prim = render_prim::quad;

	static int max_quads_per_batch;

	w_render_batch( e_render_prim render_prim );
	~w_render_batch();

	GLuint VAO_id = 0;							// vertex array object
	std::unique_ptr< w_vertex_buffer> vertex_buffer = nullptr;
	std::unique_ptr<w_index_buffer> index_buffer = nullptr;

	std::vector<int> texture_slots;
	int current_texture_slot_idx = 0;
	int assign_texture_slot( const a_texture* tex );

	void add_quad( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2, const w_render_vertex& v3 );
#if 0	// #batch
	void add_triangle( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2 );
	void add_line( const w_render_batch_vert& v0, const w_render_batch_vert& v1 );
#endif
	void bind();
	void unbind();
	void draw_and_reset();
	void reset();

private:
	void add_vert( const a_texture* tex, const w_render_vertex& render_vert );
};
