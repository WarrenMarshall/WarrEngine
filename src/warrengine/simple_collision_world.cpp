
#include "master_pch.h"
#include "master_header.h"

namespace war::simple_collision
{

world::world( vec2 gravity )
	: gravity( gravity )
{

}

/*
CUTE_C2_API int c2RaytoCircle(c2Ray A, c2Circle B, c2Raycast* out);
CUTE_C2_API int c2RaytoAABB(c2Ray A, c2AABB B, c2Raycast* out);
*/

// #raycast - should this be changed to start,normal,dist instead of start,end ?  which is more common?
void world::ray_cast( raycast_callback* callback, const vec2& start, const vec2& end ) const
{
	auto delta = ( end - start );
	auto normal = delta.normalize();
	auto dist = delta.get_size_squared();

	c2Ray ray( { start.x, start.y }, { normal.x, normal.y }, dist );

	for( auto& scc : g_engine->simple_collision.queue )
	{
	}
}

}