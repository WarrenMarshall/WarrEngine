
#include "master_pch.h"
#include "master_header.h"

w_force::w_force( w_vec2 dir, float strength )
	: dir( dir ), strength( strength )
{
}

void w_force::init_to_zero()
{
	dir = w_vec2::zero;
	strength = 0.0f;
}

// ----------------------------------------------------------------------------

// precompute whatever physics values the entity will need when doing
// collision detection.

void w_entity::update_physics()
{
	// accumulate all forces being applied to this entity and compute
	// a single vector that represents that cumuluative effect.

	physics_cache.forces = w_vec2::zero;

	for( auto& ec : components )
	{
		if( typeid(*ec.get()) == typeid(ec_force_constant) )
		{
			ec_force_constant* fec = static_cast<ec_force_constant*>( ec.get() );

			w_vec2 dir = w_vec2::from_angle( fec->angle );
			physics_cache.forces.add( w_vec2::multiply( dir, fec->strength ) );
		}
		if( typeid( *ec.get() ) == typeid( ec_force_decay ) )
		{
			ec_force_decay* fec = static_cast<ec_force_decay*>( ec.get() );

			w_vec2 dir = w_vec2::from_angle( fec->angle );
			physics_cache.forces.add( w_vec2::multiply( dir, fec->strength ) );
		}
		if( typeid( *ec.get() ) == typeid( ec_force_dir_accum ) )
		{
			ec_force_dir_accum* fec = static_cast<ec_force_dir_accum*>( ec.get() );

			w_vec2 dir = w_vec2::from_angle( fec->angle );
			physics_cache.forces.add( w_vec2::multiply( dir, fec->strength ) );
		}
	}

	physics_cache.forces.multiply( engine->time->FTS_step_value_s );

	// the position the entity has computed that it WANTS to be in after this update cycle.
	// this may change once collisions are evaluated.
	physics_cache.ending_pos = w_vec2::add( pos, physics_cache.forces );
}

void w_entity::update()
{
	// move to the physics based ending position
	pos = physics_cache.ending_pos;
}

void w_entity::update_components()
{
	// clear out dead components
	//
	// NOTE : uses standard 'for' loop because it manipulates the vector as it runs.

	for( int x = 0; x < components.size(); ++x )
	{
		w_entity_component* ec = components[ x ].get();

		if( ec->is_fully_dead() )
		{
			components.erase( components.begin() + x );
			x--;
		}
	}

	// update remaining components

	for( const auto& component : components )
	{
		MATRIX
			->push()
			->add_transform( component->pos, component->angle_facing, component->scale );

		component->update();

		MATRIX
			->pop();
	}
}

void w_entity::draw()
{
	for( const auto& component : components )
	{
		MATRIX
			->push()
			->add_transform( component->pos, component->angle_facing, component->scale );

		RENDER->push_depth_nudge();
		component->draw();

		MATRIX
			->pop();
	}
}

// let's components do anything that need to do after the entity
// is fully spawned.
//
// i.e. emitters might need to warm up
// #todo - is this needed anymore or even the right solution? feels messy

void w_entity::post_spawn()
{
	for( const auto& iter : components )
	{
		iter->post_spawn();
	}
}

void w_entity::remove_component( w_entity_component* ec )
{
	for( int x = 0 ; x < components.size() ; ++x )
	{
		if( components[x].get() == ec )
		{
			components.erase( components.begin() + x );
			return;
		}
	}

	assert( false );	// the entity component wasn't found
}

bool w_entity::can_be_deleted()
{
	// still alive, can't delete
	if( is_alive() )
	{
		return false;
	}

	if( is_dying() )
	{
		// entity is dying, but can't be deleted until all
		// components are dead

		for( const auto& iter : components )
		{
			if( !iter->is_fully_dead() )
			{
				return false;
			}
		}
	}

	// entity is fully dead, delete it
	return true;
}

void w_entity::set_transform( const w_vec2& pos, const float angle, const float scale )
{
	i_transform::set_transform( pos, angle, scale );

	// setting the transform directly means we need to sync up the physics cache to match

	physics_cache.ending_pos = pos;
}

// set the position of the entity directly, bypassing any physics or forces.

void w_entity::set_pos( const w_vec2& pos )
{
	this->pos = physics_cache.ending_pos = pos;
}

void w_entity::set_life_cycle( e_life_cycle life_cycle )
{
	i_lifecycle::set_life_cycle( life_cycle );

	if( !is_alive() )
	{
		collision_layer = 0;
		collides_with = 0;
	}

	for( const auto& iter : components )
	{
		iter->set_life_cycle( life_cycle );
	}
}

// ----------------------------------------------------------------------------

void w_entity_fx::update()
{
	w_entity::update();

	// once all of the components have died, the fx container entity can die.
	if( components.size() == 0 )
	{
		set_life_cycle( life_cycle::dying );
	}
}
