
#include "master_pch.h"
#include "master_header.h"

void w_game::new_game()
{
	entities.clear();
}

void w_game::update()
{
	for( const auto& iter : entities )
	{
		iter->update();
	}

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

void w_game::draw_entities()
{
	for( const auto& iter : entities )
	{
		MATRIX
			->push()
			->translate( iter->pos );
		iter->draw_components();
		RENDER->stats.num_entities.inc();
		MATRIX
			->pop();
	}
}
