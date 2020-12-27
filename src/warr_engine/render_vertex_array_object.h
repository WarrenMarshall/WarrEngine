
#pragma once

struct w_vertex_array_object
{
	e_render_prim render_prim = render_prim::quad;
	GLuint gl_id = 0;
	GLenum gl_prim_type = GL_TRIANGLES;

	float indices_to_verts_factor = 1.5f;

	std::unique_ptr< w_vertex_buffer> vertex_buffer = nullptr;
	std::unique_ptr<w_index_buffer> index_buffer = nullptr;

	w_vertex_array_object( e_render_prim render_prim );
	~w_vertex_array_object();

	void bind();
	void unbind();
};
