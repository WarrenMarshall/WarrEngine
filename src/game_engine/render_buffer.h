#pragma once

// ----------------------------------------------------------------------------

struct w_render_vert
{
	w_render_vert( const w_vec2& pos, const w_uv& uv, const w_color& color );

	float x, y, z;
	float u, v;
	float r, g, b, a;
};

// ----------------------------------------------------------------------------

struct w_render_buffer
{
	w_render_buffer();
	~w_render_buffer();

	static unsigned int buffer_usage;

	std::vector<w_render_vert> vertices;

	// indexes, in groups of 3 (aka triangles), indexing into the vertices array
	std::vector<unsigned int> indices;

	void add_quad( const w_render_vert& v0, const w_render_vert& v1, const w_render_vert& v2, const w_render_vert& v3 );
	void add_line( const w_render_vert& v0, const w_render_vert& v1 );
	void bind();
	void unbind();
	void draw( a_texture* tex );
	void clear();
	void log_stats( i_asset* asset );

private:

	unsigned int VAO = 0; // vertex array object
	unsigned int VBO = 0; // vertex buffer
    unsigned int EBO = 0; // index buffer

	int add_render_vert( const w_render_vert& render_vert );
};
