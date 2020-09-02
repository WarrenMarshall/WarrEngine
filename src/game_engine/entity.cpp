
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

void w_entity::pre_update()
{
	physics_cache.forces = w_vec2::zero;

	for( const auto& force : forces )
	{
		physics_cache.forces.add( w_vec2::multiply( force->dir, force->strength ) );
	}

	physics_cache.forces.multiply( engine->time->FTS_step_value_s );

	// the position the entity has computed that it WANTS to be in after this update cycle.
	// this may change once collisions are evaluated.
	physics_cache.ending_pos = w_vec2::add( pos, physics_cache.forces );
}

void w_entity::update()
{
	pos = physics_cache.ending_pos;
	log_msg( fmt::format( "{},{}", pos.x, pos.y ) );

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

void w_entity::set_life_cycle( e_lifecycle lifecycle )
{
	i_lifecycle::set_life_cycle( lifecycle );

	for( const auto& iter : components )
	{
		iter->set_life_cycle( lifecycle );
	}
}

// ----------------------------------------------------------------------------

w_entity_cozy::w_entity_cozy()
	: w_entity()
{
	life_remaining = 500;
}

void w_entity_cozy::update()
{
	w_entity::update();

	if( is_alive() )
	{
		life_remaining -= engine->time->FTS_step_value_ms;

		if( life_remaining <= 0.0f )
		{
			set_life_cycle( lifecycle::dying );
		}
	}
}
