
#include "master_pch.h"
#include "master_header.h"

bool w_layer::handle_input_event( const w_input_event* evt )
{
	return false;
}

void w_layer::update()
{
	// uses standard 'for' loop because it manipulates the
	// vector as it runs.

	for( int x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[ x ].get();

		if( e->can_be_deleted() )
		{
			entities.erase( entities.begin() + x );
			x--;
		}
	}

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

void w_layer::draw()
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
