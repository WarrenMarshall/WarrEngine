
#include "master_pch.h"

// #shader - screen wipes would be nice. just simple black bars that cover and uncover the screen in various directions.

namespace war
{

Collision_World::Collision_World( Scene* parent_scene )
	: parent_scene( parent_scene )
{
}

void Collision_World::ray_cast( collision::Raycast_Callback* callback, const Entity* entity, const Vec2& start, const Vec2& end ) const
{
	auto delta = ( end - start );
	auto ray_normal = Vec2::normalize( delta );
	auto ray_length = delta.get_size();

	c2Ray ray = {};
	ray.p = { to_c2( start.x ), to_c2( start.y ) };
	ray.d = { ray_normal.x, ray_normal.y };
	ray.t = to_c2( ray_length );

	for( auto scc : active_bodies )
	{
		auto cscc = ( Collision_Body_Component* )scc;

		// don't trace against self
		if( cscc->parent_entity == entity )
		{
			continue;
		}

		// traces don't hit sensors
		if( cscc->is_sensor() )
		{
			continue;
		}

		// if collision mask don't match, skip
		if( callback->collision_mask > 0 and ( cscc->collides_with_mask & callback->collision_mask ) )
		{
			continue;
		}

		c2Raycast raycast = {};

		switch( cscc->prim_type )
		{
			case e_sc_prim_type::circle:
			{
				if( c2RaytoCircle( ray, cscc->as_c2_circle(), &raycast ) )
				{
					raycast.t = from_c2( raycast.t );
					if( !callback->report_component( entity, ray, cscc, raycast ) )
					{
						return;
					}
				}
			}
			break;

			case e_sc_prim_type::aabb:
			{
				if( c2RaytoAABB( ray, cscc->as_c2_aabb(), &raycast ) )
				{
					raycast.t = from_c2( raycast.t );
					if( !callback->report_component( entity, ray, cscc, raycast ) )
					{
						return;
					}
				}
			}
			break;

			case e_sc_prim_type::polygon:
			{
				auto poly = cscc->as_c2_poly();
				if( c2RaytoPoly( ray, &poly, nullptr, &raycast ) )
				{
					raycast.t = from_c2( raycast.t );
					if( !callback->report_component( entity, ray, cscc, raycast ) )
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

void Collision_World::handle_collisions()
{
	// ----------------------------------------------------------------------------
	// broad phase
	//
	// look through all entities with colliders and see if they could
	// potentially collide with each other. this is done very loosely just to
	// pair down the list.
	// ----------------------------------------------------------------------------

	std::set<ec_collision_body_pair> potentially_colliding_bodies;

	for( auto& ea : parent_scene->entities )
	{
		// entities MUST be in the quad tree to participate in collisions

		if( !ea->flags.include_in_quad_tree )
		{
			continue;
		}

		auto sccs_a = ea->get_components( e_component_type::collision_body );

		if( sccs_a.empty() )
		{
			continue;
		}

		auto potential_entities = parent_scene->spatial_map.find_potentially_colliding_entities( ea.get() );

		for( auto& eb : potential_entities )
		{
			assert( ea.get() != eb );

			auto sccs_b = eb->get_components( e_component_type::collision_body );

			for( auto scc_a : sccs_a )
			{
				for( auto scc_b : sccs_b )
				{
					if( (( Collision_Body_Component* )scc_a)->does_intersect_broadly( ( Collision_Body_Component* )scc_b ) )
					{
						assert( scc_a->parent_entity != scc_b->parent_entity );
						potentially_colliding_bodies.insert( std::make_pair( ( Collision_Body_Component* )scc_a, ( Collision_Body_Component* )scc_b ) );
					}
				}
			}
		}
	}

	// ----------------------------------------------------------------------------
	// granular phase
	//
	// look through the list of potential colliders and if they actually ARE
	// colliding, handle that accordingly.
	// ----------------------------------------------------------------------------

	for( auto& [body_a, body_b] : potentially_colliding_bodies )
	{
		if( body_a->parent_entity == body_b->parent_entity )
		{
			continue;
		}

		if( body_a->flags.is_platform )
		{
			auto coll = body_a->intersects_with_manifold( body_b );
			if( !coll.has_value() )
			{
				continue;
			}
		}

		if( body_b->flags.is_platform )
		{
			auto coll = body_b->intersects_with_manifold( body_a );
			if( !coll.has_value() )
			{
				continue;
			}
		}

		if( body_a->is_solid() and body_b->is_solid() )
		{
			auto coll = body_a->intersects_with_manifold( body_b );
			if( coll.has_value() )
			{
				push_apart( *coll );
				resolve_solid_collision( *coll );
				need_another_iteration = true;
				continue;
			}
		}

		if( body_a->is_sensor() or body_b->is_sensor() )
		{
			auto coll = body_a->intersects_with_manifold( body_b );
			if( coll.has_value() )
			{
				resolve_sensor_collision( *coll );
				continue;
			}
		}
	}
}

void Collision_World::push_apart( collision::Pending_Collision& coll )
{
	if( glm::abs( coll.depth ) < settings.push_apart_tolerance )
	{
		return;
	}

	auto ent_a = coll.entity_a;
	auto ent_b = coll.entity_b;

	auto a_is_dynamic = ent_a->collision.is_dynamic();
	auto b_is_dynamic = ent_b->collision.is_dynamic();

	if( a_is_dynamic and b_is_dynamic )
	{
		ent_a->add_delta_pos( -coll.normal * coll.depth * settings.skin_thickness * 0.5f );
		ent_b->add_delta_pos( coll.normal * coll.depth * settings.skin_thickness * 0.5f );
	}
	else if( b_is_dynamic )
	{
		ent_b->add_delta_pos( coll.normal * coll.depth * settings.skin_thickness );
	}
	else
	{
		ent_a->add_delta_pos( -coll.normal * coll.depth * settings.skin_thickness );
	}
}

// Two bodies have collided. We now need to decide what to do about that.

void Collision_World::resolve_solid_collision( collision::Pending_Collision& coll )
{
	auto ent_attacker = coll.entity_a;
	auto ent_victim = coll.entity_b;

	// ----------------------------------------------------------------------------
	// give the scene the first shot at handling the collision

	if( parent_scene->on_entity_collided_with_entity( coll.entity_a, coll.entity_b, coll ) )
	{
		return;
	}

	// give entity_a a shot at handling it

	if( ent_attacker->on_collided( coll ) )
	{
		return;
	}

	// swap the entity info, and give entityb a shot at handling it

	collision::Pending_Collision coll_b = coll;
	std::swap( coll_b.entity_a, coll_b.entity_b );
	std::swap( coll_b.body_a, coll_b.body_b );

	if( ent_victim->on_collided( coll_b ) )
	{
		return;
	}

	// ----------------------------------------------------------------------------

	if( coll.normal.is_zero() )
	{
		return;
	}

	// ----------------------------------------------------------------------------

	auto vel_attacker = ent_attacker->velocity;
	auto vel_victim = ent_victim->velocity;

	if( vel_attacker.is_zero() and vel_victim.is_zero() )
	{
		return;
	}

	switch( ent_attacker->collision.type )
	{
		case e_physics_body_type::dynamic:
		{
			std::optional<Vec2> dir_attacker, dir_victim;

			switch( ent_victim->collision.type )
			{
				// dynamic -> dynamic
				case e_physics_body_type::dynamic:
				{
					if( ent_attacker->collision.flags.is_bouncy )
					{
						// #slide ?
						dir_attacker = Vec2::reflect_across_normal( vel_attacker, coll.normal );
					}
					if( ent_victim->collision.flags.is_bouncy )
					{
						// #slide ?
						dir_victim = Vec2::reflect_across_normal( vel_victim, coll.normal );
					}
				}
				break;

				// dynamic -> kinematic
				case e_physics_body_type::kinematic:
				{
					if( ent_attacker->collision.flags.is_bouncy )
					{
						// #slide ?
						dir_attacker = Vec2::reflect_across_normal( vel_attacker, coll.normal );
					}
				}
				break;

				// dynamic -> stationary
				case e_physics_body_type::stationary:
				{
					if( ent_attacker->collision.flags.is_bouncy )
					{
						// #slide ?
						ent_attacker->reflect_across( coll.normal );
					}
				}
				break;
			}

			if( dir_attacker.has_value() and !vel_attacker.is_zero() )
			{
				dir_attacker->normalize();
				ent_attacker->add_impulse( { *dir_attacker, vel_attacker.get_size() } );
				ent_attacker->velocity = Vec2::zero;
			}

			if( dir_victim.has_value() and !vel_victim.is_zero() )
			{
				dir_victim->normalize();
				ent_victim->add_impulse( { *dir_victim, vel_victim.get_size() } );
				ent_victim->velocity = Vec2::zero;
			}
		}
		break;

		case e_physics_body_type::kinematic:
		{
			Vec2 dir_victim;

			switch( ent_victim->collision.type )
			{
				// kinematic -> dynamic
				case e_physics_body_type::dynamic:
				{
					if( ent_victim->collision.flags.is_bouncy )
					{
						// #slide ?
						dir_victim = Vec2::reflect_across_normal( vel_victim, coll.normal );
					}
				}
				break;

				// kinematic -> kinematic
				case e_physics_body_type::kinematic:
				{
					assert( false );
				}
				break;

				// kinematic -> stationary
				case e_physics_body_type::stationary:
				{
					assert( false );
				}
				break;
			}

			dir_victim.normalize();

			ent_victim->add_impulse( { dir_victim, vel_victim.get_size() } );

			ent_victim->velocity = Vec2::zero;
		}
		break;

		case e_physics_body_type::stationary:
		{
			switch( ent_victim->collision.type )
			{
				// stationary -> dynamic
				case e_physics_body_type::dynamic:
				{
					assert( false );
				}
				break;

				// stationary -> kinematic
				case e_physics_body_type::kinematic:
				{
					assert( false );
				}
				break;

				// stationary -> stationary
				case e_physics_body_type::stationary:
				{
					assert( false );
				}
				break;
			}
		}
		break;
	}
}

void Collision_World::resolve_sensor_collision( collision::Pending_Collision& coll )
{
	if( coll.body_b->is_sensor() )
	{
		coll.entity_a->add_sensor_to_touch_list( coll.body_b );
	}

	if( coll.body_a->is_sensor() )
	{
		coll.entity_b->add_sensor_to_touch_list( coll.body_a );
	}
}

void Collision_World::init_sensor_sets_for_new_frame() const
{
	for( auto& entity : parent_scene->entities )
	{
		entity->sensors_last_frame = entity->sensors_this_frame;
		entity->sensors_this_frame.clear();
	}
}

void Collision_World::process_sensor_sets() const
{
	for( auto& entity : parent_scene->entities )
	{
		std::set<Collision_Body_Component*> common_set = entity->sensors_last_frame;
		common_set.insert( entity->sensors_this_frame.begin(), entity->sensors_this_frame.end() );

		for( auto sensor : common_set )
		{
			bool last_frame = entity->sensors_last_frame.contains( sensor );
			bool this_frame = entity->sensors_this_frame.contains( sensor );

			// begin

			if( !last_frame and this_frame )
			{
				// if sensor is one-shot and has already triggered, skip it
				if( sensor->sensor.is_one_shot() and sensor->sensor.is_expired() )
				{
					continue;
				}

				// give the scene a chance to handle the event, otherwise pass
				// to the entity doing the touching
				if( !parent_scene->on_entity_and_sensor_touching_begin( entity.get(), sensor ) )
				{
					entity->on_touching_begin( sensor );
				}

				// if it's a repeating sensor, set the next available time to
				// right now
				if( sensor->sensor.is_repeating() )
				{
					sensor->sensor.time_next = g_engine->clock.now();
				}
			}

			// touching

			if( last_frame and this_frame )
			{
				// if sensor is one-shot and has already triggered, skip it
				if( sensor->sensor.is_one_shot() and sensor->sensor.is_expired() )
				{
					continue;
				}

				// if this is a repeating sensor but we are still in the delay
				// period, skip it
				if( sensor->sensor.is_repeating() and g_engine->clock.now() < sensor->sensor.time_next )
				{
					continue;
				}

				// give the scene a chance to handle the event, otherwise pass
				// to the entity doing the touching
				if( !parent_scene->on_entity_and_sensor_touching( entity.get(), sensor) )
				{
					entity->on_touching( sensor );
				}

				// if this is a repeating trigger, set the next time it is
				// available to fire. otherwise mark it as triggered.
				if( sensor->sensor.is_repeating() )
				{
					sensor->sensor.time_next = g_engine->clock.now() + sensor->sensor.time_delay;
				}
				else
				{
					sensor->sensor.expired = true;
				}
			}

			// end

			if( last_frame and !this_frame )
			{
				// if sensor is one-shot and has already triggered, skip it
				if( sensor->sensor.is_one_shot() and sensor->sensor.is_expired() )
				{
					continue;
				}

				// give the scene a chance to handle the event, otherwise pass
				// to the entity doing the touching
				if( !parent_scene->on_entity_and_sensor_touching_end( entity.get(), sensor ) )
				{
					entity->on_touching_end( sensor );
				}
			}
		}
	}
}

}
