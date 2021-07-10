
#include "master_pch.h"
#include "master_header.h"

namespace war::simple_collision
{

world::world( vec2 gravity )
	: gravity( gravity )
{

}

void world::ray_cast( raycast_callback* callback, const entity* entity, const vec2& start, const vec2& end ) const
{
	auto delta = ( end - start );
	auto ray_normal = vec2::normalize( delta );
	auto ray_length = delta.get_size_squared();

	c2Ray ray = {};
	ray.p.x = start.x;
	ray.p.y = start.y;
	ray.d.x = ray_normal.x;
	ray.d.y = ray_normal.y;
	ray.t = ray_length;

	for( auto scc : entity->parent_scene->simple_collision.bodies )
	{
		// don't trace against self
		if( scc->parent_entity == entity )
		{
			continue;
		}

		// if collision mask don't match, skip
		if( callback->collision_mask > 0 and ( scc->collides_with_mask & callback->collision_mask ) )
		{
			continue;
		}

		c2Raycast raycast = {};

		switch( scc->type )
		{
			case simple_collision_type::circle:
			{
				if( c2RaytoCircle( ray, scc->as_c2_circle(), &raycast ) )
				{
					if( !callback->report_component( entity, ray, scc, raycast ) )
					{
						return;
					}
				}
			}
			break;

			case simple_collision_type::aabb:
			{
				if( c2RaytoAABB( ray, scc->as_c2_aabb(), &raycast ) )
				{
					if( !callback->report_component( entity, ray, scc, raycast ) )
					{
						return;
					}
				}
			}
			break;
		}
	}
}

}