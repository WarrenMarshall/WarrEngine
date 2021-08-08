
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

	auto ent_a = coll.entity_a;
	auto ent_b = coll.entity_b;

	auto a_is_circle = ( coll.body_a->type == sc_prim_type::circle );
	auto b_is_circle = ( coll.body_b->type == sc_prim_type::circle );

	if( ent_a->simple.is_dynamic() and ent_b->simple.is_dynamic() )
	{
		if( a_is_circle and b_is_circle )
		{
			ent_a->add_delta_pos( -coll.normal * coll.depth * simple_collision_skin_thickness * 0.5f );
			ent_b->add_delta_pos( coll.normal * coll.depth * simple_collision_skin_thickness * 0.5f );
		}
		else
		{
			ent_a->add_delta_pos( -coll.normal * coll.depth * simple_collision_skin_thickness * 0.5f );
			ent_b->add_delta_pos( coll.normal * coll.depth * simple_collision_skin_thickness * 0.5f );
		}
	}
	else
	{
		ent_a->add_delta_pos( -coll.normal * coll.depth * simple_collision_skin_thickness );
	}
}

// responds to a collision between 2 entities

void simple_collision_world::resolve_collision( simple_collision::pending_collision& coll )
{
	auto ent_a = coll.entity_a;
	auto ent_b = coll.entity_b;

	auto a_is_circle = coll.body_a->type == sc_prim_type::circle;
	auto b_is_circle = coll.body_b->type == sc_prim_type::circle;

	if( ent_a->simple.is_dynamic() and ent_b->simple.is_dynamic() )
	{
		if( a_is_circle && b_is_circle )
		{
			// circle to circle

			if( ent_a->simple.is_bouncy or ent_b->simple.is_bouncy )
			{
				auto velocity_a = ent_a->velocity;
				auto velocity_b = ent_b->velocity;

				if( ( velocity_a + velocity_b ).is_zero() )	{ return; }
				if( velocity_a.is_zero() )	{ velocity_a = velocity_b * -1.f; }
				if( velocity_b.is_zero() )	{ velocity_b = velocity_a * -1.f; }

				auto relative_velocity = velocity_b - velocity_a;

				auto dot = vec2::dot( vec2::normalize( relative_velocity ), vec2::normalize( coll.normal ) );

				if( isnan( dot ) )
				{
					log_fatal( "damn it" );
				}

				auto total_velocity = velocity_a.get_size() + velocity_b.get_size();
				auto new_dir_a = vec2::reflect_across_normal( velocity_a, coll.normal );
				auto new_dir_b = vec2::reflect_across_normal( velocity_b, coll.normal );

				auto total_mass = ent_a->simple.mass + ent_b->simple.mass;
				auto mass_pct_a = ent_a->simple.mass / total_mass;

				ent_a->reset_force( { new_dir_a, total_velocity * ( 1.0f - mass_pct_a ) } );
				ent_b->reset_force( { new_dir_b, total_velocity * mass_pct_a } );
			}
		}
		else
		{
			// circle to box/polygon

			if( ent_a->simple.is_bouncy or ent_b->simple.is_bouncy )
			{
				auto velocity_a = ent_a->velocity;
				auto velocity_b = ent_b->velocity;

				if( ( velocity_a + velocity_b ).is_zero() )	{ return; }
				if( velocity_a.is_zero() )	{ velocity_a = velocity_b * -1.f; }
				if( velocity_b.is_zero() )	{ velocity_b = velocity_a * -1.f; }

				auto relative_velocity = velocity_b - velocity_a;

				auto dot = vec2::dot( vec2::normalize( relative_velocity ), vec2::normalize( coll.normal ) );

				if( isnan( dot ) )
				{
					log_fatal( "damn it" );
				}

				auto total_velocity = velocity_a.get_size() + velocity_b.get_size();
				auto new_dir_a = vec2::reflect_across_normal( velocity_a, coll.normal );
				auto new_dir_b = vec2::reflect_across_normal( velocity_b, coll.normal );

				auto total_mass = ent_a->simple.mass + ent_b->simple.mass;
				auto mass_pct_a = ent_a->simple.mass / total_mass;

				ent_a->reset_force( { new_dir_a, total_velocity * ( 1.0f - mass_pct_a ) } );
				ent_b->reset_force( { new_dir_b, total_velocity * mass_pct_a } );
			}
		}
	}
	else
	{
		// dynamic vs stationary

		if( ent_a->simple.is_bouncy or ent_b->simple.is_bouncy )
		{
			ent_a->reflect_across( coll.normal );
		}
	}

#if 1
	if( !ent_a->simple.is_bouncy )
	{
		if( ent_a->simple.is_dynamic() and ent_b->simple.is_stationary() )
		{
			// when landing on the ground, kill any velocity on the Y axis. this
			// stops it from accruing to the maximum as you run around on flat
			// geo.

			if( coll.normal.y < -0.75f or coll.normal.y > 0.75f )
			{
				ent_a->velocity.y = 0.f;
			}

			// hitting a wall kills horizontal velocity
			if( coll.normal.x < -0.75f or coll.normal.x > 0.75f )
			{
				ent_a->velocity.x = 0.f;
			}
		}
	}
#endif
}

}
