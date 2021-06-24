
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

// #raycast - should this be changed to start,normal,dist instead of start,end ? which is more useful?
bool world::ray_cast( raycast_callback* callback, const entity* entity, const vec2& start, const vec2& end ) const
{
	bool hit_something = false;

	auto delta = ( end - start );
	auto ray_normal = delta.normalize();
	auto ray_length = delta.get_size_squared();

	c2Ray ray( { start.x, start.y }, { ray_normal.x, ray_normal.y }, ray_length );

	log( "-- ray casting --" );

	for( auto scc : entity->parent_scene->simple_collision_components )
	{
		// don't trace against self
		if( scc->parent_entity == entity )
		{
			continue;
		}

		// if collision mask don't match, skip
		if ( callback )
		{
			if( callback->collision_mask == 0 or ( scc->collides_with_mask & callback->collision_mask ) )
			{
				continue;
			}
		}

		c2Raycast rc = {};

		switch( scc->type )
		{
			case simple_collision_type::circle:
			{
				if( c2RaytoCircle( ray, scc->as_c2_circle(), &rc ) )
				{
					hit_something = true;
					if( callback )
					{
						callback->hit_something = true;
					}
					log( "hit circle : {},{} : {}", rc.n.x, rc.n.y, rc.t );
				}
			}
			break;

			case simple_collision_type::aabb:
			{
				if( c2RaytoAABB( ray, scc->as_c2_aabb(), &rc ) )
				{
					hit_something = true;
					if( callback )
					{
						callback->hit_something = true;
					}
					log( "hit aabb : {},{} : {}", rc.n.x, rc.n.y, rc.t );
				}
			}
			break;
		}
	}

	log( "-- ----------- --" );

	return hit_something;
}

}