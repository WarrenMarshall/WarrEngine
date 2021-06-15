
#include "master_pch.h"
#include "master_header.h"

namespace war
{

render_vertex::render_vertex( const vec2& pos, const vec2& uv, const color& color, float glow )
	: x( pos.x ), y( pos.y ), z( render::state->z ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	glow( glow )
{
}

render_vertex::render_vertex( const vec2& pos )
	: x( pos.x ), y( pos.y ), z( render::state->z ),
	u( 0.f ), v( 0.f ),
	r( render::state->color.r ), g( render::state->color.g ), b( render::state->color.b ), a( render::state->color.a ),
	glow( render::state->glow )
{
}

render_vertex::render_vertex( const vec3& pos, const vec2& uv, const color& color, float glow )
	: x( pos.x ), y( pos.y ), z( pos.z + render::state->z ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	glow( glow )
{
}

render_vertex::render_vertex( const vec3& pos )
	: x( pos.x ), y( pos.y ), z( pos.z + render::state->z ),
	u( 0.f ), v( 0.f ),
	r( 1.f ), g( 1.f ), b( 1.f ), a( 1.f ),
	glow( 0.f )
{
}

render_vertex::render_vertex( const render_vertex& other )
{
	x = other.x;
	y = other.y;
	z = other.z;
	u = other.u;
	v = other.v;
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
	glow = other.glow;
	texture_id = other.texture_id;
	pick_id = other.pick_id;
}

// ----------------------------------------------------------------------------

render_triangle::render_triangle( const render_vertex& v0, const render_vertex& v1, const render_vertex& v2, const color& color, float glow )
{
	auto l_copy_render_vert = [&] ( int idx, const render_vertex& rv )
	{
		verts[ idx ] = rv;
		verts[ idx ].r = color.r;
		verts[ idx ].g = color.g;
		verts[ idx ].b = color.b;
		verts[ idx ].a = color.a;
		verts[ idx ].glow = glow;
	};

	l_copy_render_vert( 0, v0 );
	l_copy_render_vert( 1, v1 );
	l_copy_render_vert( 2, v2 );
}


// ----------------------------------------------------------------------------

render_line::render_line( const render_vertex& v0, const render_vertex& v1, const color& color, float glow )
{
	auto l_copy_render_vert = [&] ( int idx, const render_vertex& rv )
	{
		verts[ idx ] = rv;
		verts[ idx ].r = color.r;
		verts[ idx ].g = color.g;
		verts[ idx ].b = color.b;
		verts[ idx ].a = color.a;
		verts[ idx ].glow = glow;
	};

	l_copy_render_vert( 0, v0 );
	l_copy_render_vert( 1, v1 );
}

}
