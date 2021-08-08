
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

entity_simple_force::entity_simple_force( vec2 normal, float strength )
	: strength( strength )
{
	this->normal = vec2::normalize( normal );
}

// ----------------------------------------------------------------------------

void entity::update_from_physics()
{
	update_transform_to_match_physics_components();
}

void entity::pre_update()
{
	life_cycle.pre_update();

	if( simple.is_affected_by_gravity )
	{
		apply_force( { vec2::down, fixed_time_step::per_second( simple_collision_gravity_default ) } );
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

void entity::apply_force( const entity_simple_force& force )
{
	// forces are gradual, and build up over time
	pending_forces.emplace_back( force.normal, fixed_time_step::per_second( force.strength ) );
}

void entity::apply_impulse( const entity_simple_force& force )
{
	// impulses are immediate, full strength changes
	pending_forces.push_back( force );
}

void entity::compile_velocity()
{
	for( auto& f : pending_forces )
	{
		velocity += f.normal * f.strength;
	}

	velocity_dir = vec2::normalize( velocity );
	velocity_strength = velocity.get_size();

	velocity.clamp( simple.max_velocity );

	pending_forces.clear();
}

void entity::reset_force( const entity_simple_force& force )
{
	// reverse out the current velocity first
	velocity = vec2::zero;

	// then add the new velocity
	apply_impulse( force );
}

void entity::reflect_across( vec2 normal )
{
	auto reflected_dir = vec2::reflect_across_normal( velocity, normal.normalize() );

	if( !reflected_dir.is_zero() )
	{
		reset_force( { reflected_dir, velocity.get_size() } );
	}
}

void entity::apply_forces()
{
	// figure out where this entity should be moving
	compile_velocity();

	// move it
	add_delta_pos( velocity );

	// apply friction to the velocity
	velocity.x = lerp(
		velocity.x,
		0.f,
		simple.friction
	);

	if( !simple.is_affected_by_gravity )
	{
		velocity.y = lerp(
			velocity.y,
			0.f,
			simple.friction
		);
	}

	if( velocity.get_size() < 0.01f )
	{
		velocity = vec2::zero;
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

		scoped_opengl;
		g_engine->render_api.top_matrix->apply_transform( component->get_pos(), component->get_angle(), component->get_scale() );

		render::state->z += zdepth_nudge;

		scoped_render_state;

		render::state->set_from_opt( rs_opt );
		render::state->pick_id = pick_id;
		component->draw();
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

/*
void entity::on_collided( simple_collision::pending_collision& coll )
{
	if( auto scr = get_component<ec_scr_push_outside>() ; scr )
	{
		scr->begin();
		scr->on_collided( coll );
		scr->end();
	}
}

void entity::on_touched( simple_collision::pending_collision& coll )
{
}
*/

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
