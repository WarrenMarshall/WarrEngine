
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

	for( auto scc : active_bodies )
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
			case sc_prim_type::circle:
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

			case sc_prim_type::aabb:
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

			case sc_prim_type::polygon:
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

// loop through all active bodies and check them against each other.
//
// each set of bodies that collides needs to:
// - push themselves apart so they aren't colliding anymore
// - react to the collision (changing direction, or whatever)

void simple_collision_world::generate_collision_set()
{
	colliding_bodies_set.clear();

	// broad phase

	for( auto scc_a : active_bodies )
	{
		for( auto scc_b : active_bodies )
		{
			if( scc_a->intersects_with( scc_b ) )
			{
				// the bodies are touching so add them into the contact list if the pair is unique

				colliding_bodies_set.insert( std::make_pair( scc_a, scc_b ) );
			}
		}
	}

	// generate detailed information about the collisions

	//pending_collisions.clear();
	//pending_touches.clear();

	for( auto& [body_a, body_b] : colliding_bodies_set )
	{
		switch( body_a->collider_type )
		{
			case sc_body_collider_type::solid:
			{
				auto coll = body_a->intersects_with_manifold( body_b );
				if( coll.has_value() )
				{
					push_apart( *coll );
					resolve_collision( *coll );
					need_another_iteration = true;
				}
			}
			break;

			case sc_body_collider_type::sensor:
			{
				auto coll = body_a->intersects_with_manifold( body_b );
				if( coll.has_value() )
				{
					//pending_touches.push_back( *coll );
				}
			}
			break;
		}
	}
}

// pushes two entities apart so they are no longer intersecting

void simple_collision_world::push_apart( simple_collision::pending_collision& coll )
{
	if( glm::abs( coll.depth ) < 0.01f )
	{
		return;
	}

	auto a_is_circle = ( coll.body_a->type == sc_prim_type::circle );
	auto b_is_circle = ( coll.body_b->type == sc_prim_type::circle );

	if( coll.entity_a->simple.is_dynamic() and coll.entity_b->simple.is_dynamic() )
	{
		if( a_is_circle and b_is_circle )
		{
			coll.entity_a->add_delta_pos( -coll.normal * ( coll.depth * simple_collision_skin_thickness * 0.5f ) );
			coll.entity_b->add_delta_pos( coll.normal * ( coll.depth * simple_collision_skin_thickness * 0.5f ) );
		}
		else
		{
			coll.entity_a->add_delta_pos( -coll.normal * ( coll.depth * simple_collision_skin_thickness * 0.5f ) );
			coll.entity_b->add_delta_pos( coll.normal * ( coll.depth * simple_collision_skin_thickness * 0.5f ) );
		}
	}
	else
	{
		coll.entity_a->add_delta_pos( -coll.normal * ( coll.depth * simple_collision_skin_thickness ) );
	}
}

// responds to a collision between 2 entities

void simple_collision_world::resolve_collision( simple_collision::pending_collision& coll )
{
	auto a_is_circle = ( coll.body_a->type == sc_prim_type::circle );
	auto b_is_circle = ( coll.body_b->type == sc_prim_type::circle );

	if( coll.entity_a->simple.is_dynamic() and coll.entity_b->simple.is_dynamic() )
	{
		if( a_is_circle and b_is_circle )
		{
			coll.entity_a->reflect_across( -coll.normal );
			coll.entity_b->reflect_across( coll.normal );
		}
		else
		{
			//coll.entity_a->reflect_across( -coll.normal );
			//coll.entity_b->reflect_across( coll.normal );
		}
	}
	else
	{
		coll.entity_a->reflect_across( -coll.normal );
	}

	if( coll.entity_a->simple.is_dynamic() and coll.entity_b->simple.is_stationary() )
	{
		// when landing on the ground, kill any velocity on the Y axis. this
		// stops it from accruing to the maximum as you run around on flat
		// geo.

		if( coll.normal.y < -0.75f or coll.normal.y > 0.75f )
		{
			coll.entity_a->velocity.y = 0.f;
		}

		// hitting a wall kills horizontal velocity
		if( coll.normal.x < -0.75f or coll.normal.x > 0.75f )
		{
			coll.entity_a->velocity.x = 0.f;
		}
	}

}

}


/*
	if( sc_type_a == sc_type::dynamic and sc_type_b == sc_type::stationary )
	{
		// when landing on the ground, kill any velocity on the Y axis. this
		// stops it from accruing to the maximum as you run around on flat
		// geo.

		if( coll.normal.y < -0.75f or coll.normal.y > 0.75f )
		{
			parent_entity->velocity.y = 0.f;
		}

		// hitting a wall kills horizontal velocity
		if( coll.normal.x < -0.75f or coll.normal.x > 0.75f )
		{
			parent_entity->velocity.x = 0.f;
		}
	}

	// dynamic->dynamic - slow down movement

	if( sc_type_a == sc_type::dynamic and sc_type_b == sc_type::dynamic )
	{
		if( coll.normal.y < -0.75f or coll.normal.y > 0.75f )
		{
			parent_entity->velocity.y *= 0.75f;
		}

		if( coll.normal.x < -0.75f or coll.normal.x > 0.75f )
		{
			parent_entity->velocity.x *= 0.75f;
		}
	}
*/
