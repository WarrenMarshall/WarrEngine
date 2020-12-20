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
};

// ----------------------------------------------------------------------------

struct w_render_batch
{
	w_render_batch();
	~w_render_batch();

	std::vector<w_render_batch_vert> vertices;
	std::vector<unsigned short> indices;	// indexes, in groups of 3 (aka triangles), indexing into the vertices array
	GLuint VAO = 0;							// vertex array object
	GLuint VBO = 0;							// vertex buffer
	GLuint EBO = 0;							// index buffer

	void add_quad( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2, const w_render_batch_vert& v3 );
	void add_triangle( const w_render_batch_vert& v0, const w_render_batch_vert& v1, const w_render_batch_vert& v2 );
	void add_line( const w_render_batch_vert& v0, const w_render_batch_vert& v1 );
	void bind();
	void unbind();
	void draw( a_texture* tex );
	void clear();

	unsigned short add_render_vert( const w_render_batch_vert& render_vert );
};
