
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

	if( auto mc = get_component<ec_movement_controller>() ; mc )
	{
		if( mc->affected_by_gravity )
		{
			add_force( vec2::down, fixed_time_step::per_second( simple_collision_gravity_default ) );
		}
	}

	apply_forces();
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
}

void entity::add_force( vec2 force, float strength )
{
	pending_forces.emplace_back( force, strength );
}

void entity::add_force_x( float strength )
{
	pending_forces.emplace_back( vec2::right, strength );
}

void entity::add_force_y( float strength )
{
	pending_forces.emplace_back( vec2::up, strength );
}

void entity::compile_velocity()
{
	for( auto& f : pending_forces )
	{
		velocity += f.n * f.strength;
	}

	pending_forces.clear();

	limit_velocity();
}

void entity::limit_velocity()
{
	if( auto mc = get_component<ec_movement_controller>() ; mc )
	{
		velocity = mc->clamp_velocity( velocity );
	}
}

void entity::reset_force( vec2 force, float strength )
{
	// reverse out the current velocity first
	add_force( vec2::normalize( velocity ), -( velocity.get_size() ) );
	// then add the new velocity
	add_force( force, strength );
}

void entity::reset_force_x( float strength )
{
	for( auto& f : pending_forces )
	{
		f.n.x = 0.f;
	}

	add_force( vec2( 1.f, 0.f ), strength );
}

void entity::reset_force_y( float strength )
{
	for( auto& f : pending_forces )
	{
		f.n.y = 0.f;
	}

	add_force( vec2( 0.f, 1.f ), strength );
}

void entity::apply_forces()
{
	compile_velocity();
	add_delta_pos( velocity );

	if( auto mc = get_component<ec_movement_controller>() ; mc )
	{
		velocity.x = lerp( velocity.x, 0.f, fixed_time_step::per_second( mc->horizontal_damping * simple_collision_max_friction ) );

		if( !mc->affected_by_gravity )
		{
			velocity.y = lerp( velocity.y, 0.f, fixed_time_step::per_second( mc->vertical_damping * simple_collision_max_friction ) );
		}
	}
}

// ----------------------------------------------------------------------------

void entity::draw()
{
	// super hacky way to indicate a selected entity. it just pumps up the glow
	// on it. we need a nicer system for this eventually.
	rs_opt.glow = is_selected * 2.f;

	for( const auto& component : components )
	{
		if( component->life_cycle.is_dead() )
		{
			continue;
		}

		{
			scoped_opengl;
			g_engine->render_api.top_matrix->apply_transform( component->get_pos(), component->get_angle(), component->get_scale() );

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

	if( has_component<ec_box2d_physics>() )
	{
		std::vector<ec_box2d_physics_body*> ecs;
		get_components<ec_box2d_physics_body, ec_box2d_dynamic_physics_body>( ecs );
		get_components<ec_box2d_physics_body, ec_box2d_kinematic_physics_body>( ecs );

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

	std::vector<ec_box2d_physics_body*> ecs;
	get_components<ec_box2d_physics_body, ec_box2d_dynamic_physics_body>( ecs );
	get_components<ec_box2d_physics_body, ec_box2d_kinematic_physics_body>( ecs );

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
	assert( !has_component<ec_box2d_physics>() );

	return &_tform;
}

vec2 entity::get_pos()
{
	return get_transform()->pos;
}

float entity::get_angle()
{
	return get_transform()->angle;
}

float entity::get_scale()
{
	return get_transform()->scale;
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

	// note : without this check, box2d bodies stop reacting to gravity - not
	// sure why, but for now, leave this alone.
	if( !delta.is_zero() )
	{
		update_physics_components_to_match_transform();
	}

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
	assert( !has_component<ec_box2d_physics>() );

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

void entity::on_collided( simple_collision::pending_collision& coll )
{
	if( auto scr = get_component<ec_simple_collision_responder>() ; scr )
	{
		scr->begin();
		scr->on_collided( coll );
		scr->end();
	}
}

void entity::on_touched( simple_collision::pending_collision& coll )
{
	if( auto scr = get_component<ec_simple_collision_responder>() ; scr )
	{
		if( auto mc = get_component<ec_movement_controller>() ; mc )
		{
			mc->in_air = false;
		}
	}
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

	if( auto ec = get_component<ec_box2d_physics>() ; ec and !life_cycle.is_alive() )
	{
		ec->clear_collision_flags();
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
