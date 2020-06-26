
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_entity::w_entity()
	: i_transform()
{
}

w_entity::~w_entity()
{
}

void w_entity::update()
{
	for( auto& iter : components )
	{
		iter->update();
	}
}

void w_entity::draw_components()
{
	for( auto const& iter : components )
	{
		OPENGL
			->push( false )
			->add_transform( ( *iter.get() ) );
		iter->draw();
		OPENGL
			->pop();
	}
}

void w_entity::draw()
{
	// custom drawing code that entities might want to do
	//
	// entity can assume that it's transform has been applied
	// and that drawing is taking place at the modelview origin
}

// let's components do anything that need to do after the entity
// is fully spawned.
//
// i.e. emitters might need to warm up

void w_entity::post_spawn()
{
	for( auto const& iter : components )
	{
		iter->post_spawn();
	}
}

/*
	takes a worldspace position and checks to see if it's
	within the bounds of this entities collision bounds
*/

bool w_entity::bounds_contains_point( w_vec3 pos_to_check )
{
	if( bounds->contains_pt( pos_to_check ) )
	{
		return true;
	}

	return false;
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

		for( auto& iter : components )
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

	for( auto& iter : components )
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

w_entity_cozy::~w_entity_cozy()
{
}

void w_entity_cozy::update()
{
	w_entity::update();

	if( is_alive() )
	{
		life_remaining -= engine->time->FTS_step_value_ms;

		if( life_remaining <= 0.0f )
		{
			set_life_cycle( e_lifecycle::dying );
		}
	}
}
