
#include "master_pch.h"
#include "master_header.h"

w_layer::w_layer()
{
	draws_completely_solid = false;
	blocks_further_input = false;

#ifndef _FINALRELEASE
	is_debug_physics_layer = false;
#endif

	// preallocate some space for entity pointers
	entities.reserve( 150 );
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

	// update entities and components

	for( auto& entity : entities )
	{
		OPENGL
			->push()
			->add_transform( *( entity->get_tform() ) );

		entity->update();
		entity->update_components();
		entity->update_from_physics();

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
			->add_transform( *entity->get_tform() );

		entity->draw();

		OPENGL
			->pop();
	}
}

void w_layer::draw_ui()
{
}

w_entity* w_layer::get_camera()
{
	return find_entity( "main_camera" );
}

bool w_layer::is_topmost_layer() const
{
	return ( LAYER == this );
}

w_entity* w_layer::find_entity( const std::string& tag )
{
	for( auto& iter : entities )
	{
		if( iter->tag == tag )
		{
			return iter.get();
		}
	}

	return nullptr;
}

void w_layer::new_game()
{
	entities.clear();
}

w_imgui_callback* w_layer::get_imgui_callback()
{
	return nullptr;
}
