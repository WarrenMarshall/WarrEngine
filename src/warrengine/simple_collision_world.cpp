
#include "master_pch.h"
#include "master_header.h"

namespace war
{

simple_collision_world::simple_collision_world( scene* parent_scene )
	: parent_scene( parent_scene )
{
}

void simple_collision_world::ray_cast( simple_collision::raycast_callback* callback, const entity* entity, const vec2& start, const vec2& end ) const
{
	auto delta = ( end - start );
	auto ray_normal = vec2::normalize( delta );
	auto ray_length = delta.get_size();

	c2Ray ray = {};
	ray.p.x = to_simple( start.x );
	ray.p.y = to_simple( start.y );
	ray.d.x = ray_normal.x;
	ray.d.y = ray_normal.y;
	ray.t = to_simple( ray_length );

	for( auto scc : bodies )
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
				if( c2RaytoCircle( ray, scc->as_simple_circle(), &raycast ) )
				{
					raycast.t = from_simple( raycast.t );
					if( !callback->report_component( entity, ray, scc, raycast ) )
					{
						return;
					}
				}
			}
			break;

			case simple_collision_type::aabb:
			{
				if( c2RaytoAABB( ray, scc->as_simple_aabb(), &raycast ) )
				{
					raycast.t = from_simple( raycast.t );
					if( !callback->report_component( entity, ray, scc, raycast ) )
					{
						return;
					}
				}
			}
			break;

			case simple_collision_type::polygon:
			{
				auto poly = scc->as_simple_poly();
				if( c2RaytoPoly( ray, &poly, nullptr, &raycast ) )
				{
					raycast.t = from_simple( raycast.t );
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

void simple_collision_world::pre_update()
{
	// this list can change between updates, so it needs to be recreated each
	// time. entities get deleted, created, change their collision masks, etc.

	bodies.clear();
}

void simple_collision_world::update( )
{
	for( auto& entity : parent_scene->entities )
	{
		scoped_opengl;

		auto tform = entity->get_transform();
		g_engine->render_api.top_matrix->apply_transform( tform->pos, tform->angle, tform->scale );

		// collect the simple collision bodies active in the scene
		auto sccs = entity->get_components<ec_simple_collision_body>();
		bodies.insert(
			bodies.end(),
			sccs.begin(), sccs.end()
		);
	}
}

// loop through all collision bodies that are available for collision and check
// them against each other. for each unique set that collides, add them into the
// queue for response processing later.

void simple_collision_world::generate_colliding_bodies_set()
{
	colliding_bodies_set.clear();

	// broad phase

	for( auto scc_a : bodies )
	{
		for( auto scc_b : bodies )
		{
			if( scc_a->intersects_with( scc_b ) )
			{
				// the bodies are touching so add them into the contact list if the pair is unique

				colliding_bodies_set.insert( std::make_pair( scc_a, scc_b ) );
			}
		}
	}

	// generate detailed information about the collisions

	pending_collisions.clear();
	pending_touches.clear();

	for( auto& [body_a, body_b] : colliding_bodies_set )
	{
		switch( body_a->collider_type )
		{
			case simple_collider_type::solid:
			{
				pending_collisions.push_back( body_a->intersects_with_manifold( body_b ) );
				need_another_iteration = true;
			}
			break;

			case simple_collider_type::sensor:
			{
				pending_touches.push_back( body_a->intersects_with_manifold( body_b ) );
			}
			break;
		}
	}
}

void simple_collision_world::respond_to_pending_simple_collisions()
{
	for( auto& coll : pending_collisions )
	{
		coll.entity_a->on_collided( coll );
	}

	for( auto& coll : pending_touches )
	{
		coll.entity_a->on_touched( coll );
	}
}


}