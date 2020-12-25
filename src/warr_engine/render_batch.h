#pragma once

struct w_render_batch
{
	e_render_prim render_prim = render_prim::quad;
	GLenum gl_prim_type = GL_TRIANGLES;

	int max_elements_per_batch = 10000;
	float indices_to_verts_factor = 1.5f;

	w_render_batch( e_render_prim render_prim );
	~w_render_batch();

	GLuint VAO_id = 0;		// vertex array object
	std::unique_ptr< w_vertex_buffer> vertex_buffer = nullptr;
	std::unique_ptr<w_index_buffer> index_buffer = nullptr;

	std::vector<const a_texture*> texture_slots;
	int current_texture_slot_idx = 0;
	int assign_texture_slot( const a_texture* tex );

	void add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2, const w_render_vertex& v3 );
	void add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1, const w_render_vertex& v2 );
	void add_primitive( const a_texture* tex, const w_render_vertex& v0, const w_render_vertex& v1 );
	void add_primitive( const a_texture* tex, const w_render_vertex& v0 );

	void bind();
	void unbind();
	void check_draw_and_reset();
	void draw_and_reset();
	void reset();

private:
	void add_vert( const a_texture* tex, const w_render_vertex& render_vert );
};
