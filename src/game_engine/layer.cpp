

#include "master_pch.h"
#include "master_header.h"

bool w_layer::handle_input_event( const w_input_event* evt )
{
	return false;
}

void w_layer::update()
{
	// clear out dead entities
	//
	// NOTE : uses standard 'for' loop because it manipulates the vector as it runs.

	for( int x = 0; x < entities.size(); ++x )
	{
		w_entity* e = entities[ x ].get();

		if( e->can_be_deleted() )
		{
			entities.erase( entities.begin() + x );
			x--;
		}
	}

	// update the remaining entities

	for( const auto& entity : entities )
	{
		//MATRIX
		//	->push()
		//	->add_transform( entity->pos, entity->angle_facing, entity->scale );

		entity->pre_update();

		//MATRIX
		//	->pop();
	}

	update_collisions();

	for( const auto& entity : entities )
	{
		MATRIX
			->push()
			->add_transform( entity->pos, entity->angle_facing, entity->scale );

		entity->update();

		MATRIX
			->pop();
	}
}

void w_layer::update_collisions()
{
}

void w_layer::draw()
{
	for( const auto& entity : entities )
	{
		MATRIX
			->push()
			->add_transform( entity->pos, entity->angle_facing, entity->scale );

		entity->draw();
		RENDER->stats.num_entities.inc();

		MATRIX
			->pop();
	}
}

bool w_layer::is_topmost_layer()
{
	return ( engine->layer_mgr->get_top() == this );
}
