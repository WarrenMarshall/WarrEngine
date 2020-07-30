
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_entity::update()
{
	for( const auto& iter : components )
	{
		iter->update();
	}
}

void w_entity::draw_components()
{
	for( const auto& iter : components )
	{
		RENDER->push_depth_nudge();
		iter->draw();
	}
}

// let's components do anything that need to do after the entity
// is fully spawned.
//
// i.e. emitters might need to warm up

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
