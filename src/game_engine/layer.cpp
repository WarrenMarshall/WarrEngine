

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

	// update all entities as per physics

	for( const auto& entity : entities )
	{
		entity->update_physics();
	}

	// update entities and components

	for( const auto& entity : entities )
	{
		MATRIX
			->push()
			->add_transform( entity->pos, entity->angle_facing, entity->scale );

		entity->update();
		entity->update_components();

		MATRIX
			->pop();
	}
}

#if 0
void w_layer::update_collisions()
{
	int num_entities = static_cast<int>( entities.size() );
	for( int e1 = 0 ; e1 < num_entities ; ++e1 )
	{
		w_entity* ent = entities[ e1 ].get();

		if( ent->collides_with > 0 )
		{
			for( int e2 = 0 ; e2 < num_entities ; ++e2 )
			{
				w_entity* ent2 = entities[ e2 ].get();
				if( e1 != e2 )
				{
					if( ent->collides_with & ent2->collision_layer )
					{
						auto colliders = ent->get_components<ec_collider>();
						for( auto& collider : colliders )
						{
							auto colliders2 = ent2->get_components<ec_collider>();
							for( auto& collider2 : colliders2 )
							{
								variant_collider_types coll = collider->get_collider();
								variant_collider_types coll2 = collider2->get_collider();

								if( c2Collided( &coll, NULL, collider->c2type,
												&coll2, NULL, collider2->c2type ) )
								{
									c2Manifold hit;

									coll = collider->get_collider();
									coll2 = collider2->get_collider();
									c2Collide( &coll, NULL, collider->c2type,
											   &coll2, NULL, collider2->c2type,
											   &hit );

									if( hit.count )
									{
										ent->collided_with( collider, collider2->parent_entity, hit );
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
#endif

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
