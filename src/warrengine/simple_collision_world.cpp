
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
					//need_another_iteration = true;
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

void simple_collision_world::push_apart( simple_collision::pending_collision& coll )
{
	if( coll.entity_a->sc_type == sc_type::dynamic and coll.entity_b->sc_type == sc_type::dynamic )
	{
		auto a_is_circle = ( coll.body_a->type == sc_prim_type::circle );
		auto b_is_circle = ( coll.body_b->type == sc_prim_type::circle );

		if( a_is_circle and b_is_circle )
		{
			auto hit_normal = vec2( coll.body_a->ws.pos ) - vec2( coll.closest_point );
			hit_normal = vec2::normalize( hit_normal );

			auto dir_a = vec2::reflect_across_normal( coll.entity_a->velocity, hit_normal );
			auto dir_b = vec2::reflect_across_normal( coll.entity_b->velocity, hit_normal );

			// ----------------------------------------------------------------------------
			// push apart

			assert( coll.depth > 0.f );
			coll.entity_a->add_force( -coll.normal, coll.depth * 1.0f );
			coll.entity_b->add_force( coll.normal, coll.depth * 1.0f );

		#if 0
			// ----------------------------------------------------------------------------
			// resolve collision

			auto relative_velocity = coll.entity_b->velocity - coll.entity_a->velocity;
			auto dot = vec2::dot( relative_velocity, coll.normal );
			//auto dot = vec2::dot( vec2::normalize( coll.entity_b->velocity ), vec2::normalize( coll.entity_a->velocity ) );

			log( "dot : {}", dot );
			if( dot > 0.0f )
			{

				//auto e = 0.0f;
				auto j = dot;

				vec2 impulse = coll.normal * j;

				//coll.entity_a->add_force( -coll.normal, 1.0f );
				//coll.entity_b->add_force( coll.normal, 1.0f );
				coll.entity_a->add_force( vec2::normalize( impulse ), impulse.get_size() );
				coll.entity_b->add_force( -vec2::normalize( impulse ), impulse.get_size() );

			}
		#endif
		}
		else
		{
			coll.entity_a->reset_force( -coll.normal, coll.depth * 0.5f );
			coll.entity_b->reset_force( coll.normal, coll.depth * 0.5f );
		}
	}
	else
	{
		coll.entity_a->add_force( -coll.normal, coll.depth );
	}

	//coll.entity_b->add_delta_pos( { coll.normal * ( coll.depth * simple_collision_skin_thickness ) } );
}

void simple_collision_world::resolve_collision( entity* a, entity* b )
{

}

}