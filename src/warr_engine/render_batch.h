#pragma once

constexpr int max_elements_per_render_batch = 10000;

struct w_render_batch
{
	w_render_batch( e_render_prim render_prim );

	std::unique_ptr<w_vertex_array_object> vertex_array_object;

	void add_primitive( const a_texture* texture, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2, const w_render_vertex& v3 );
	void add_primitive( const a_texture* texture, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2 );
	void add_primitive( const a_texture* texture, const w_render_vertex& v0, const w_render_vertex& v1 );
	void add_primitive( const a_texture* texture, const w_render_vertex& v0 );

	void draw_and_reset();
	void draw_and_reset_internal();

	bool is_empty();

private:
	void add_vert( const a_texture* texture, const w_render_vertex& render_vert );
};
