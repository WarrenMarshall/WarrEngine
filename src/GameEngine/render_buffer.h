#pragma once

// ----------------------------------------------------------------------------

struct w_render_vert
{
	w_render_vert( const w_vec2& pos, const w_uv& uv, const w_color& color );

	float x, y, z;
	float u, v;
	float r, g, b, a;

    bool const is_same( const w_render_vert& other )
    {
		assert( false );	// this shouldn't be getting called from anywhere - why are you here?
        if( fequals( x, other.x )
            && fequals( y, other.y )
            && fequals( z, other.z )
            && fequals( u, other.u )
            && fequals( v, other.v )
            && fequals( r, other.r )
            && fequals( g, other.g )
            && fequals( b, other.b )
            && fequals( a, other.a ) )
        {
            return true;
        }

        return false;
    }
};

// ----------------------------------------------------------------------------

struct w_render_buffer
{
	w_render_buffer() = delete;
	w_render_buffer( unsigned int prim_type );
	~w_render_buffer();

	unsigned int prim_type = GL_TRIANGLES;
	unsigned int usage = GL_DYNAMIC_DRAW;

	std::vector<w_render_vert> vertices[2];

	// indexes, in groups of 3 (aka triangles), into vertices array
	std::vector<unsigned int> indices[2];

	void add_quad( const w_render_vert& v0, const w_render_vert& v1, const w_render_vert& v2, const w_render_vert& v3 );
	void add_line( const w_render_vert& v0, const w_render_vert& v1 );
	void bind();
	void unbind();
	void draw( int render_pass );
	void clear();
	void log_stats( i_asset* asset );

private:

	unsigned int VAO = 0; // vertex array object
	unsigned int VBO = 0; // vertex buffer
    unsigned int EBO = 0; // index buffer

	int add_render_vert( int render_pass, const w_render_vert& render_vert );
};
