
#include "master_pch.h"
#include "master_header.h"

w_game::w_game()
{
}

void w_game::new_game()
{
	entities.clear();
}

void w_game::update()
{
	for( int x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[ x ].get();
		e->update();
	}

	for( int x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[x].get();

		if( e->can_be_deleted() )
		{
			entities.erase( entities.begin() + x );
			x--;
		}
		else
		{
			e->update();
		}
	}
}

void w_game::draw_entities()
{
	for( int x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[x].get();

		OPENGL
			->push( false )
			->add_transform( *e );

		e->draw_components();
		e->draw();

		OPENGL->pop();

		RENDER->stats.num_entities.inc();
	}
}
