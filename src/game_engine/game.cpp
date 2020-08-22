
#include "master_pch.h"
#include "master_header.h"

void w_game::new_game()
{
	entities.clear();
}

void w_game::update()
{
	update_entities();

	// uses standard 'for' loop because it manipulates the
	// vector as it runs.

	for( int x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[x].get();

		if( e->can_be_deleted() )
		{
			entities.erase( entities.begin() + x );
			x--;
		}
	}
}

void w_game::update_entities()
{
	for( const auto& entity : entities )
	{
		MATRIX
			->push()
			->translate( entity->pos );

		entity->update();

		MATRIX
			->pop();
	}
}

void w_game::draw_entities()
{
	for( const auto& entity : entities )
	{
		MATRIX
			->push()
			->translate( entity->pos );

		entity->draw();
		RENDER->stats.num_entities.inc();
		
		MATRIX
			->pop();
	}
}
