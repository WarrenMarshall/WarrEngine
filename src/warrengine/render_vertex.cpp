
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Render_Vertex::Render_Vertex( const Vec2& pos, const Vec2& uv, const Color& color, f32 glow )
	: x( pos.x ), y( pos.y ), z( Render::state->z + Render::state->z_bias ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	glow( glow )
{
}

Render_Vertex::Render_Vertex( const Vec2& pos )
	: x( pos.x ), y( pos.y ), z( Render::state->z + Render::state->z_bias ),
	u( 0.f ), v( 0.f ),
	r( Render::state->color.r ), g( Render::state->color.g ), b( Render::state->color.b ), a( Render::state->color.a ),
	glow( Render::state->glow )
{
}

Render_Vertex::Render_Vertex( const Vec3& pos, const Vec2& uv, const Color& color, f32 glow )
	: x( pos.x ), y( pos.y ), z( pos.z + Render::state->z + Render::state->z_bias ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	glow( glow )
{
}

Render_Vertex::Render_Vertex( const Vec3& pos )
	: x( pos.x ), y( pos.y ), z( pos.z + Render::state->z + Render::state->z_bias ),
	u( 0.f ), v( 0.f ),
	r( 1.f ), g( 1.f ), b( 1.f ), a( 1.f ),
	glow( 0.f )
{
}

Render_Vertex::Render_Vertex( const Render_Vertex& other )
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

Render_Triangle::Render_Triangle( const Render_Vertex& v0, const Render_Vertex& v1, const Render_Vertex& v2, const Color& color, f32 glow )
{
	auto l_copy_render_vert = [&] ( size_t idx, const Render_Vertex& rv )
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

}
