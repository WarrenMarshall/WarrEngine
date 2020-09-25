
#include "master_pch.h"
#include "master_header.h"

void w_entity::update_physics()
{
	// entities with box2d components need their transforms
	// updated as per what the physics engine is reporting.
	//
	// NOTE : there should be only one "primary body" attached
	// to an entity so it is assumed that once we find and
	// process that one, we're done.
	//
	// NOTE : this ignores the fact that you may have multiple
	// fixtures attached to a body and assumes that the body
	// position/rotation is representative of where the
	// entity is in the world.

	for( auto& ec : components )
	{
		if( ec->type & (component_type::b2d_dynamic | component_type::b2d_kinematic) )
		{
			ec_b2d_body* edb = static_cast<ec_b2d_body*>( ec.get() );
			if( edb->is_primary_body )
			{
				b2Vec2 position = edb->body->GetPosition();

				position.x = from_b2d( position.x );
				position.y = from_b2d( position.y );

				float angle = edb->body->GetAngle();

				set_transform( { position.x, position.y }, rad2deg( angle ), scale );

				break;
			}
		}
	}
}

void w_entity::update()
{
}

void w_entity::update_components()
{
	// clear out dead components
	//
	// NOTE : uses standard 'for' loop because it manipulates the vector as it runs.

	for( int x = 0; x < components.size(); ++x )
	{
		w_entity_component* ec = components[ x ].get();

		if( ec->is_fully_dead() )
		{
			components.erase( components.begin() + x );
			x--;
		}
	}

	// update remaining components

	for( const auto& component : components )
	{
		MATRIX
			->push()
			->add_transform( component->pos, component->angle_facing, component->scale );

		component->update();

		MATRIX
			->pop();
	}
}

void w_entity::draw()
{
	for( const auto& component : components )
	{
		MATRIX
			->push()
			->add_transform( component->pos, component->angle_facing, component->scale );

		RENDER->push_depth_nudge();
		component->draw();

		MATRIX
			->pop();
	}
}

// immediately moves the entity and it's dynamic/kinematic bodies to a new position.

void w_entity::teleport( w_vec2 pos, bool reset_velocity )
{
	auto ecs = get_components<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

	for( auto ec : ecs )
	{
		ec->body->SetTransform( pos.to_b2d(), 0 );

		if( reset_velocity )
		{
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );
		}

		ec->body->SetAwake( true );
	}
}

void w_entity::remove_component( w_entity_component* ec )
{
	for( int x = 0 ; x < components.size() ; ++x )
	{
		if( components[x].get() == ec )
		{
			components.erase( components.begin() + x );
			return;
		}
	}

	assert( false );	// the entity component wasn't found
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

void w_entity::set_collision( e_collision_layer layer, e_collision_layer collides_with )
{
	this->collision_layer = layer;
	this->collides_with = collides_with;
}

void w_entity::set_life_cycle( e_life_cycle life_cycle )
{
	i_life_cycle::set_life_cycle( life_cycle );

	if( !is_alive() )
	{
		collision_layer = 0;
		collides_with = 0;
	}

	for( const auto& iter : components )
	{
		iter->set_life_cycle( life_cycle );
	}
}

// ----------------------------------------------------------------------------

void w_entity_fx::update()
{
	w_entity::update();

	// once all of the components have died, the fx container entity can die.
	if( components.size() == 0 )
	{
		set_life_cycle( life_cycle::dying );
	}
}
