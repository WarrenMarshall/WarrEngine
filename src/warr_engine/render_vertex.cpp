
#include "master_pch.h"
#include "master_header.h"

w_render_vertex::w_render_vertex( const w_vec2& pos, const w_uv& uv, const w_color& color, const float emissive )
	: x( pos.x ), y( pos.y ), z( RENDER->rs_z_depth ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	e( emissive )
{
}

w_render_vertex::w_render_vertex( const w_vec3& pos, const w_uv& uv, const w_color& color, const float emissive )
	: x( pos.x ), y( pos.y ), z( pos.z + RENDER->rs_z_depth ),
	u( uv.u ), v( uv.v ),
	r( color.r ), g( color.g ), b( color.b ), a( color.a ),
	e( emissive )
{
}
