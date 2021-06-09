
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

render_vertex::render_vertex( const vec3& pos, const vec2& uv, const color& color, float glow )
	: x( pos.x ), y( pos.y ), z( pos.z + render::state->z ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	glow( glow )
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

}
