
#include "master_pch.h"
#include "master_header.h"

w_entity::w_entity()
{
	// all entities have a transform by default
	add_component<ec_transform>();
}

void w_entity::update_from_physics()
{
	// entities with box2d components need their transforms
	// updated as per what the physics engine is reporting.
	//
	// NOTE : there should be only one "primary body" attached
	// to an entity so it is assumed that once we find and
	// process that one, we're done.

	std::vector<ec_b2d_body*> ecs;
	get_components<ec_b2d_body, ec_b2d_dynamic>( ecs );
	get_components<ec_b2d_body, ec_b2d_kinematic>( ecs );

	for( auto& ec : ecs )
	{
		if( ec->is_primary_body )
		{
			auto tform = get_transform();
			w_vec2 position = w_vec2( ec->body->GetPosition() ).from_b2d();
			float angle = ec->body->GetAngle();

			tform->set( { position.x, position.y }, glm::degrees( angle ), tform->scale );
			break;
		}
	}
}

void w_entity::update()
{
	// tick the death delay down, stopping at zero
	ilc_update_count_death_delay = glm::max( ilc_update_count_death_delay - 1, 0 );
}

void w_entity::update_components()
{
	// clear out dead components
	// NOTE : uses standard 'for' loop because it manipulates the vector as it runs.

	for( size_t x = 0; x < components.size(); ++x )
	{
		w_entity_component* ec = components[ x ].get();

		if( ec->is_fully_dead() && !ec->is_permanent() )
		{
			components.erase( components.begin() + x );
			x--;
		}
	}

	// update remaining components

	for( const auto& component : components )
	{
		{
			scoped_opengl;
			OPENGL->top()->add_transform( component->tform );

			component->update();
		}
	}
}

void w_entity::draw()
{
	for( const auto& component : components )
	{
		{
			scoped_opengl;
			OPENGL->top()->add_transform( component->tform );

			render_state.z += zdepth_nudge;

			{
				scoped_render_state;

				render_state.set_from_opt( rs_opt );
				render_state.pick_id = pick_id;
				component->draw();
			}
		}
	}
}

// immediately moves the entity and it's dynamic/kinematic bodies to a new position.

void w_entity::set_pos_deep( const w_vec2& pos, bool reset_velocity )
{
	// entity
	get_transform()->set_pos( pos );

	// physics components
	std::vector<ec_b2d_body*> ecs;
	get_components<ec_b2d_body, ec_b2d_dynamic>( ecs );
	get_components<ec_b2d_body, ec_b2d_kinematic>( ecs );

	for( auto ec : ecs )
	{
		ec->body->SetTransform( pos.to_b2d().as_b2Vec2(), 0 );

		if( reset_velocity )
		{
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );
		}

		ec->body->SetAwake( true );
	}
}

void w_entity::set_angle_deep( float angle )
{
	// entity
	get_transform()->set_angle( angle );

	// physics components
	std::vector<ec_b2d_body*> ecs;
	get_components<ec_b2d_body, ec_b2d_dynamic>( ecs );
	get_components<ec_b2d_body, ec_b2d_kinematic>( ecs );

	for( auto ec : ecs )
	{
		ec->body->SetTransform( ec->body->GetTransform().p, glm::radians( angle ) );
		ec->body->SetAwake( true );
	}
}

// entities are starting to collide

void w_entity::on_collision_begin( w_pending_collision& coll, w_entity* other )
{
}

// entities are no longer colliding

void w_entity::on_collision_end( w_pending_collision& coll, w_entity* other )
{
}

w_transform* w_entity::get_transform()
{
	return &( get_component<ec_transform>()->tform );
}

bool w_entity::can_be_deleted()
{
	// still alive, can't delete
	if( ilc_is_alive() )
	{
		return false;
	}

	if( ilc_update_count_death_delay )
	{
		return false;
	}

	if( ilc_is_dying() )
	{
		// entity is dying, but can't be deleted until all
		// components are dead.

		for( const auto& iter : components )
		{
			if( !iter->is_permanent() )
			{
				if( !iter->is_fully_dead() )
				{
					return false;
				}
			}
		}
	}

	// entity is fully dead, delete it
	return true;
}

static float w_entity_last_pick_id = 0.0f;

void w_entity::make_pickable()
{
	w_entity_last_pick_id += 1.0f;
	pick_id = w_entity_last_pick_id;
}

void w_entity::ilc_set( e_life_cycle life_cycle )
{
	i_life_cycle::ilc_set( life_cycle );

	if( !ilc_is_alive() )
	{
		get_component<ec_physics>()->clear_collision_flags();
	}

	for( const auto& iter : components )
	{
		iter->ilc_set( life_cycle );
	}
}

// ----------------------------------------------------------------------------

void w_entity_transient::update()
{
	w_entity::update();

	// once all of the components have died, the fx container entity can die.
	if( components.empty() )
	{
		ilc_set( life_cycle::dying );
	}
}

// ----------------------------------------------------------------------------

e_camera::e_camera()
	: w_entity()
{
	tag = H( "main_camera" );
}
