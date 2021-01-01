
#pragma once

struct w_vertex_array_object
{
	e_render_prim render_prim = render_prim::quad;
	GLuint gl_id = 0;
	GLenum gl_prim_type = GL_TRIANGLES;
	w_render_batch* batch = nullptr;

	float indices_to_verts_factor = 1.5f;

	std::unique_ptr<w_vertex_buffer> vertex_buffer = nullptr;
	std::unique_ptr<w_index_buffer> index_buffer = nullptr;

	w_vertex_array_object( w_render_batch* batch, e_render_prim render_prim );
	~w_vertex_array_object();

	void bind();
	void unbind();
	int assign_texture_slot( const a_texture* texture );
	void check_draw_and_reset();
	void draw_and_reset();
	void reset();
};