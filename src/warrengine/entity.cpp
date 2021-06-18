
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void entity::update_from_physics()
{
	update_transform_to_match_physics_components();
}

void entity::pre_update()
{
	life_cycle.pre_update();
}

void entity::update()
{
}

void entity::update_components()
{
	// clear out dead components
	// NOTE : uses standard 'for' loop because it manipulates the vector as it runs.

	for( size_t x = 0; x < components.size(); ++x )
	{
		entity_component* ec = components[ x ].get();

		if( ec->is_fully_dead() )
		{
			components.erase( components.begin() + x );
			x--;
		}
	}

	// update remaining components

	for( const auto& component : components )
	{
		if( component->life_cycle.is_dead() )
		{
			continue;
		}

		{
			scoped_opengl;
			g_engine->render_api.top_matrix->apply_transform( *component->get_transform() );

			component->update();
		}
	}
}

void entity::post_update()
{
}

// ----------------------------------------------------------------------------

void entity::draw()
{
	for( const auto& component : components )
	{
		if( component->life_cycle.is_dead() )
		{
			continue;
		}

		{
			scoped_opengl;
			g_engine->render_api.top_matrix->apply_transform( component->get_transform()->pos, component->get_transform()->angle, component->get_transform()->scale );

			render::state->z += zdepth_nudge;

			{
				scoped_render_state;

				render::state->set_from_opt( rs_opt );
				render::state->pick_id = pick_id;
				component->draw();
			}
		}
	}
}

// called whenever the transform for an entity is changed. if there are physics
// components attached, they are updated to match the entity transform.
//
// this is generally not done during gameplay, but more as a set up thing. if
// you have physics, let the physics engine control what the entity transform is
// via the regular update() cycle.

void entity::update_physics_components_to_match_transform()
{
	if( has_component<box2d_physics_component>() )
	{
		std::vector<box2d_physics_body_component*> ecs;
		get_components<box2d_physics_body_component, box2d_dynamic_physics_body_component>( ecs );
		get_components<box2d_physics_body_component, box2d_kinematic_physics_body_component>( ecs );

		for( auto ec : ecs )
		{
			ec->body->SetTransform( _tform.pos.to_box2d().to_b2Vec2(), glm::radians( _tform.angle ) );

			// reset velocity - #physics : might want to make this an option somehow
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );

			ec->body->SetAwake( true );
		}
	}
}

void entity::update_transform_to_match_physics_components()
{
	// entities with physics components need their transforms
	// updated as per what the physics engine is reporting.
	//
	// NOTE : there should be only one "primary body" attached
	// to an entity so it is assumed that once we find and
	// process that one, we're done.

	std::vector<box2d_physics_body_component*> ecs;
	get_components<box2d_physics_body_component, box2d_dynamic_physics_body_component>( ecs );
	get_components<box2d_physics_body_component, box2d_kinematic_physics_body_component>( ecs );

	for( auto& ec : ecs )
	{
		if( ec->is_primary_body )
		{
			vec2 position = vec2( ec->body->GetPosition() ).from_box2d();
			float angle = ec->body->GetAngle();


			_tform.set_pos( { position.x, position.y } );
			_tform.set_angle( glm::degrees( angle ) );
			break;
		}
	}
}

transform* entity::transform_set( const vec2& pos, const float angle, const float scale )
{
	transform_set_pos( pos );
	transform_set_angle( angle );
	transform_set_scale( scale );

	return &_tform;
}

transform* entity::transform_set_pos( const vec2& pos )
{
	_tform.set_pos( pos );

	update_physics_components_to_match_transform();

	return &_tform;
}

transform* entity::transform_set_angle( const float angle )
{
	_tform.set_angle( angle );
	update_physics_components_to_match_transform();

	return &_tform;
}

transform* entity::transform_set_scale( const float scale )
{
	_tform.set_scale( scale );

	if( has_component<box2d_physics_component>() )
	{
		// scaling an entity with physics components isn't encouraged as we
		// don't have a way to scale the physics components at the same time. so
		// collision will remain the original size which is likely not what you
		// want.
		assert( false );
	}


	return &_tform;
}

transform* entity::transform_delta_pos( const vec2& delta )
{
	_tform.add_pos( delta );
	update_physics_components_to_match_transform();
	return &_tform;
}

transform* entity::transform_delta_angle( const float delta )
{
	_tform.add_angle( delta );
	update_physics_components_to_match_transform();
	return &_tform;
}

transform* entity::transform_delta_scale( const float delta )
{
	_tform.add_scale( delta );


	if( has_component<box2d_physics_component>() )
	{
		// scaling an entity with physics components isn't encouraged as we
		// don't have a way to scale the physics components at the same time. so
		// collision will remain the original size which is likely not what you
		// want.
		assert( false );
	}

	return &_tform;
}

// entities are starting to collide

void entity::on_box2d_collision_begin( box2d_physics::pending_collision& coll, entity* touched_by )
{
}

// entities are no longer colliding

void entity::on_box2d_collision_end( box2d_physics::pending_collision& coll, entity* touched_by )
{
}

// entities have hit each other using simple collision checks

void entity::on_simple_collision( simple_collision::pending_collision& coll, entity* touched_by )
{
}

bool entity::can_be_deleted()
{
	// still alive, can't delete
	if( life_cycle.is_alive() )
	{
		return false;
	}

	if( life_cycle.death_delay )
	{
		return false;
	}

	if( life_cycle.is_dying() )
	{
		// entity is dying, but can't be deleted until all
		// components are dead.

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

void entity::set_tag( hash tag )
{
	this->tag = tag;
}

static int w_entity_last_pick_id = 0;

void entity::make_pickable()
{
	w_entity_last_pick_id++;
	pick_id = w_entity_last_pick_id;
}

void entity::set_life_cycle( e_life_cycle lc )
{
	life_cycle.set( lc );

	auto ecp = get_component<box2d_physics_component>();
	if( ecp and !life_cycle.is_alive() )
	{
		ecp->clear_collision_flags();
	}

	for( const auto& iter : components )
	{
		iter->life_cycle.set( lc );
	}
}

const life_cycle_mgr* entity::get_life_cycle()
{
	return &life_cycle;
}

// ----------------------------------------------------------------------------

void entity_transient::update()
{
	entity::update();

	// once all of the components have died, the fx container entity can die.
	if( components.empty() )
	{
		set_life_cycle( life_cycle::dying );
	}
}

}
