
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

void entity::pre_update_components()
{
	for( const auto& component : components )
	{
		scoped_opengl;
		g_engine->render_api.top_matrix->apply_transform( *component->get_transform() );

		component->pre_update();
	}
}

void entity::update_components()
{
	remove_dead_components();

	for( const auto& component : components )
	{
		scoped_opengl;
		g_engine->render_api.top_matrix->apply_transform( *component->get_transform() );

		component->update();
	}
}

void entity::post_update_components()
{
	for( const auto& component : components )
	{
		scoped_opengl;
		g_engine->render_api.top_matrix->apply_transform( *component->get_transform() );

		component->post_update();
	}
}

void entity::post_update()
{
	apply_linear_forces();
}

void entity::apply_linear_forces()
{
#if 1
	for( auto& lf : linear_forces )
	{
		linear_force_accum += lf;
		auto signs = vec2( glm::sign( linear_force_accum.x ), glm::sign( linear_force_accum.y ) );
		auto int_force = vec2( (int)linear_force_accum.x, (int)linear_force_accum.y );

		linear_force_accum -= int_force;

		while( int_force.x )
		{
			auto desired_pos = get_transform()->pos + vec2( signs.x, 0.f );
			//if( parent_scene->can_fit( this, desired_pos ) )
			{
				set_pos( desired_pos );
			}
			int_force.x -= 1.0f * signs.x;
		}

		while( int_force.y )
		{
			auto desired_pos = get_transform()->pos + vec2( 0.f, signs.y );
			//if( parent_scene->can_fit( this, desired_pos ) )
			{
				set_pos( desired_pos );
			}
			int_force.y -= 1.0f * signs.y;
		}
	}

	linear_forces.clear();
#else
	for( auto& lf : linear_forces )
	{
		linear_force_accum += lf;
		auto int_force = vec2( (int)linear_force_accum.x, (int)linear_force_accum.y );

		if( int_force.x or int_force.y )
		{
			add_delta_pos( int_force );
			linear_force_accum -= int_force;
		}
	}

	linear_forces.clear();
#endif
}

// ----------------------------------------------------------------------------

void entity::draw()
{
	// super hacky way to indicate a selected entity. it just pumps up the glow
	// on it. we need a nicer system for this eventually.
	rs_opt.glow = is_selected * 2.0f;

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
	// physics

	if( has_component<box2d_physics_component>() )
	{
		std::vector<box2d_physics_body_component*> ecs;
		get_components<box2d_physics_body_component, box2d_dynamic_physics_body_component>( ecs );
		get_components<box2d_physics_body_component, box2d_kinematic_physics_body_component>( ecs );

		for( auto ec : ecs )
		{
			ec->body->SetTransform( _tform.pos.to_box2d().to_b2Vec2(), glm::radians( _tform.angle ) );

			// reset velocity
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

void entity::add_linear_force( vec2 force )
{
	linear_forces.push_back( force );
}

const war::transform* entity::get_transform()
{
	return &_tform;
}

transform* entity::set_pos_angle_scale( const vec2& pos, const float angle, const float scale )
{
	_tform.set_pos( pos );
	_tform.set_angle( angle );
	_tform.set_scale( scale );

	update_physics_components_to_match_transform();

	return &_tform;
}

transform* entity::set_angle( const float angle )
{
	_tform.set_angle( angle );
	update_physics_components_to_match_transform();

	return &_tform;
}

transform* entity::set_scale( const float scale )
{
	_tform.set_scale( scale );

	// scaling an entity with physics components isn't encouraged as we
	// don't have a way to scale the physics components at the same time. so
	// collision will remain the original size which is likely not what you
	// want.
	assert( !has_component<box2d_physics_component>() );

	return &_tform;
}

transform* entity::set_pos( const vec2& pos )
{
	_tform.set_pos( pos );

	update_physics_components_to_match_transform();

	return &_tform;
}

transform* entity::add_delta_pos( const vec2& delta )
{
	_tform.add_pos( delta );

	update_physics_components_to_match_transform();

	return &_tform;
}

transform* entity::add_delta_angle( const float delta )
{
	_tform.add_angle( delta );

	update_physics_components_to_match_transform();

	return &_tform;
}

transform* entity::add_delta_scale( const float delta )
{
	_tform.add_scale( delta );


	// scaling an entity with physics components isn't encouraged as we
	// don't have a way to scale the physics components at the same time. so
	// collision will remain the original size which is likely not what you
	// want.
	assert( !has_component<box2d_physics_component>() );

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

void entity::remove_dead_components()
{
	for( size_t x = 0; x < components.size(); ++x )
	{
		entity_component* ec = components[ x ].get();

		if( ec->is_fully_dead() )
		{
			components.erase( components.begin() + x );
			x--;
		}
	}
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
