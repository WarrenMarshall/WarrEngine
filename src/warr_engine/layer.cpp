
#include "master_pch.h"
#include "master_header.h"

w_layer::w_layer()
{
	draws_completely_solid = false;
	blocks_further_input = false;

#ifndef _FINALRELEASE
	is_debug_physics_layer = false;
#endif
}

void w_layer::update()
{
	// clear out dead entities
	//
	// NOTE : uses standard 'for' loop because it manipulates the vector as it runs.

	for( auto x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[ x ].get();

		if( e->can_be_deleted() )
		{
			entities.erase( entities.begin() + x );
			x--;
		}
	}

	// update all entities as per the physics engine

	for( const auto& entity : entities )
	{
		entity->update_from_physics();
	}

	// update entities and components

	for( const auto& entity : entities )
	{
		OPENGL
			->push()
			->add_transform( entity->pos, entity->angle, entity->scale );

		entity->update();
		entity->update_components();

		OPENGL
			->pop();
	}
}

void w_layer::draw()
{
	for( const auto& entity : entities )
	{
		RENDER->stats.entities.inc();

		OPENGL
			->push()
			->add_transform( entity->pos, entity->angle, entity->scale );

		entity->draw();

		OPENGL
			->pop();
	}
}

void w_layer::draw_ui()
{
}

e_camera* w_layer::get_camera()
{
	return find_from_tag<e_camera>( "main_camera" );
}

bool w_layer::is_topmost_layer() const
{
	return ( LAYER == this );
}

void w_layer::new_game()
{
	entities.clear();
}

w_imgui_callback* w_layer::get_imgui_callback()
{
	return nullptr;
}
