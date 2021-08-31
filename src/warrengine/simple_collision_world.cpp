
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Simple_Collision_World::Simple_Collision_World( Scene* parent_scene )
	: parent_scene( parent_scene )
{
}

void Simple_Collision_World::ray_cast( simple_collision::Raycast_Callback* callback, const Entity* entity, const Vec2& start, const Vec2& end ) const
{
	auto delta = ( end - start );
	auto ray_normal = Vec2::normalize( delta );
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
			case e_sc_prim_type::circle:
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

			case e_sc_prim_type::aabb:
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

			case e_sc_prim_type::polygon:
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

void Simple_Collision_World::handle_collisions()
{
	colliding_bodies_set.clear();

	// broad phase

	for( auto scc_a : active_bodies )
	{
		for( auto scc_b : active_bodies )
		{
			if( scc_a->intersects_with( scc_b ) )
			{
				// if the bodies are touching and we haven't seen this pair
				// before, add them into the set
				//
				// this prevents us from processing the same collision set twice
				// every time. so basically avoids "A hit B" and then "B hit A"
				// - we only care that "A hit B".

				if( !colliding_bodies_set.contains( std::make_pair( scc_a, scc_b ) )
					and !colliding_bodies_set.contains( std::make_pair( scc_b, scc_a ) ) )
				{
					colliding_bodies_set.insert( std::make_pair( scc_a, scc_b ) );
				}
			}
		}
	}

	for( auto& [body_a, body_b] : colliding_bodies_set )
	{
		if( body_a->is_platform )
		{
			auto coll = body_a->intersects_with_manifold( body_b );
			if( !coll.has_value() )
			{
				continue;
			}
		}

		if( body_b->is_platform )
		{
			auto coll = body_b->intersects_with_manifold( body_a );
			if( !coll.has_value() )
			{
				continue;
			}
		}

		{
			switch( body_a->collider_type )
			{
				case e_sc_body_collider_type::solid:
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

				// #collision - it feels like sensors are going to have to do some
				// sort of batching thing so we only fire them ONCE per update cycle
				// and not every time through the collision iteration

				case e_sc_body_collider_type::sensor:
				{
					auto coll = body_a->intersects_with_manifold( body_b );
					if( coll.has_value() )
					{
						resolve_touch( *coll );
					}
				}
				break;
			}
		}
	}
}

// pushes two entities apart so they are no longer intersecting

void Simple_Collision_World::push_apart( simple_collision::Pending_Collision& coll )
{
	if( glm::abs( coll.depth ) < settings.push_apart_tolerance )
	{
		return;
	}

	auto ent_a = coll.entity_a;
	auto ent_b = coll.entity_b;

	auto a_is_dynamic = ent_a->simple.is_dynamic();
	auto a_is_kinematic = ent_a->simple.is_kinematic();
	auto b_is_dynamic = ent_b->simple.is_dynamic();
	auto b_is_kinematic = ent_b->simple.is_kinematic();

	auto dynamic_count = a_is_dynamic + b_is_dynamic;
	auto kinematic_count = a_is_kinematic + b_is_kinematic;;

	if( dynamic_count == 2 )
	{
		ent_a->add_delta_pos( -coll.normal * coll.depth * settings.skin_thickness * 0.5f );
		ent_b->add_delta_pos( coll.normal * coll.depth * settings.skin_thickness * 0.5f );
	}
	else if( dynamic_count == 1 and kinematic_count == 1 )
	{
		if( a_is_dynamic )
		{
			ent_a->add_delta_pos( -coll.normal * coll.depth * settings.skin_thickness );
		}
		else
		{
			ent_b->add_delta_pos( coll.normal * coll.depth * settings.skin_thickness );
		}
	}
	else
	{
		ent_a->add_delta_pos( -coll.normal * coll.depth * settings.skin_thickness );
	}
}

// responds to a collision between 2 entities

void Simple_Collision_World::resolve_collision( simple_collision::Pending_Collision& coll )
{
	auto ent_a = coll.entity_a;
	auto ent_b = coll.entity_b;

	// tell entity_a about the collision

	if( ent_a->on_collided( coll ) )
	{
		return;
	}

	// swap the entity info, and then tell entityb about the collision

	simple_collision::Pending_Collision coll_b = coll;
	std::swap( coll_b.entity_a, coll_b.entity_b );
	std::swap( coll_b.body_a, coll_b.body_b );

	if( ent_b->on_collided( coll_b ) )
	{
		return;
	}

	// if we're here, then the entities didn't fully handle the collision and we
	// should resolve it using the default behaviors

	if( ent_a->simple.is_dynamic() and ent_b->simple.is_dynamic() )
	{
		// ----------------------------------------------------------------------------
		// dynamic-to-dynamic

		if( ent_a->simple.is_bouncy or ent_b->simple.is_bouncy )
		{
			// ----------------------------------------------------------------------------
			// bouncy

			auto velocity_a = ent_a->velocity;
			auto velocity_b = ent_b->velocity;

			// #prevent_nan_town
			if( velocity_a.is_zero() or velocity_b.is_zero() ) { return; }

			auto dot = Vec2::dot( velocity_a, velocity_b );

			// entities are heading in the same direction, so swap their
			// velocities and exit. this is a cheap way to resolve that
			// situation.

			if( dot > 0.f )
			{
				ent_a->set_force( { velocity_b, velocity_b.get_size() } );
				ent_b->set_force( { velocity_a, velocity_a.get_size() } );

				return;
			}

			{
				// #prevent_nan_town
				if( velocity_a.is_zero() ) { velocity_a = velocity_b * -1.f; }
				if( velocity_b.is_zero() ) { velocity_b = velocity_a * -1.f; }
			}

			// if we've made it this far, the entities need their velocities
			// mirrored around the collision normal and then each takes half the
			// force of the impact

			auto relative_velocity = velocity_b - velocity_a;
			auto total_velocity = velocity_a.get_size() + velocity_b.get_size();
			auto new_dir_a = Vec2::reflect_across_normal( velocity_a, coll.normal );
			auto new_dir_b = Vec2::reflect_across_normal( velocity_b, coll.normal );

			ent_a->set_force( { new_dir_a, total_velocity * 0.5f } );
			ent_b->set_force( { new_dir_b, total_velocity * 0.5f } );
		}
	}
	else
	{
		// ----------------------------------------------------------------------------
		// dynamic-to-stationary

		if( ent_a->simple.is_bouncy or ent_b->simple.is_bouncy )
		{
			ent_a->reflect_across( coll.normal );
		}
	}

	// if an entity is bouncy and it uses gravity, then we need to dampen it's
	// vertical velocity each time we compiled the velocity, otherwise it'll
	// just bounce forever at the same height.

	if( ent_a->simple.is_bouncy and ent_a->simple.is_affected_by_gravity )
	{
		// hitting the ceiling or the floor means we need to dampen next update
		if( coll.normal.y < -0.75f or coll.normal.y > 0.75f )
		{
			ent_a->simple.bounce_needs_dampening = true;
			ent_b->simple.bounce_needs_dampening = true;

			// zeroing these out makes things a little more stable. but remember
			// that this isn't a real physics simulator. close enough is good
			// enough.

			ent_a->velocity = Vec2::zero;
			ent_b->velocity = Vec2::zero;
		}

	}
	else
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
}

void Simple_Collision_World::resolve_touch( simple_collision::Pending_Collision& coll )
{
	coll.entity_a->simple.is_in_air = false;
	coll.entity_a->on_touched( coll );
}

}
