
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

entity::entity()
{
}

entity::entity( std::string debug_name )
{
#ifdef _DEBUG
	this->debug_name = debug_name;
#endif
}

void entity::update_from_physics()
{
	update_transform_to_match_physics_components();
}

void entity::pre_update()
{
	life_cycle.pre_update();

	simple.is_in_air = true;

	if( simple.is_affected_by_gravity )
	{
		float accel = 1.0f;

		// if entity is already falling downwards, accelerate the fall. this
		// makes jumping feel tighter.
		if( velocity.y > 0.f )
		{
			accel = 2.0f;
		}

		add_force( { vec2::y_axis, simple_collision_gravity_default * accel } );
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

#ifndef _FINAL_RELEASE

	if( g_engine->renderer.debug.is_entity_info_logging() )
	{

		auto tform = get_transform();
		if( debug_name.length() )
		{
			log( "{}", debug_name );
		}
		log( "P : {:.2f}, {:.2f}", tform->pos.x, tform->pos.y );
		log( "V : {:.2f}, {:.2f}", velocity.x, velocity.y );
		log( "---" );
	}
#endif
}

void entity::post_update()
{
}

// forces are gradual and build up over time.
//
// this is good for moving a character or applying things like gravity or wind.
void entity::add_force( const entity_simple_force& force )
{
	pending_forces.emplace_back( force.normal, fixed_time_step::per_second( force.strength ) );
}

// resets the velocity to zero, and applies an impulse of the requested force.
// creates an immediate change in direction.
void entity::set_force( const entity_simple_force& force )
{
	// kill the current velocity...
	//
	// #simple - do we really need to zero this out? if this can be removed,
	// this entire function can be deleted and replaced with calls to
	// apply_impulse instead.
	velocity = vec2::zero;

	// ...then add the new impulse
	add_impulse( force );
}

// impulses are immediate and applied at full strength.
//
// this is for things like jumping or bouncing off of things.
void entity::add_impulse( const entity_simple_force& force )
{
	pending_forces.push_back( force );
}

void entity::compile_velocity()
{
	for( auto& f : pending_forces )
	{
		velocity += f.normal * f.strength;
	}

	velocity.x = simple.max_velocity_x.clamp_value( velocity.x );
	velocity.y = simple.max_velocity_y.clamp_value( velocity.y );

	pending_forces.clear();
}

void entity::reflect_across( vec2 normal )
{
	auto reflected_dir = vec2::reflect_across_normal( velocity, normal.normalize() );

	if( !reflected_dir.is_zero() )
	{
		set_force( { reflected_dir, velocity.get_size() } );
	}
}

void entity::apply_forces()
{
	compile_velocity();

	// if the velocity has reached a point where it's so small we're just jittering, clear it out.
/*
	if( simple.is_dynamic() && velocity.get_size() < 0.05f )
	{
		velocity = vec2::zero;
		return;
	}
*/

	add_delta_pos( velocity );

	// apply friction to the horizontal velocity
	velocity.x = lerp(
		velocity.x,
		0.f,
		simple.friction
	);

	// only apply friction to the vertical axis if we are not affected by gravity
	if( !simple.is_affected_by_gravity )
	{
		velocity.y = lerp(
			velocity.y,
			0.f,
			simple.friction
		);
	}

	// "bounce_needs_dampening" flag gets set when the entity is bouncy and it
	// hit something last frame.

	if( simple.bounce_needs_dampening )
	{
		simple.bounce_needs_dampening = false;

		// only dampen the vertical velocity on the way up
		if( velocity.y < 0.0f )
		{
			velocity.y *= 0.5f;
		}
	}
}

// ----------------------------------------------------------------------------

void entity::draw()
{
	// super hacky way to indicate a selected entity. it just pumps up the glow
	// on it. we need a nicer system for this eventually.
	rs_opt.glow = rs_opt.glow.value_or( 0.f ) + ( is_selected * 2.f );

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

// returns TRUE if the collision was handled
bool entity::on_collided( simple_collision::pending_collision& coll )
{
	return false;
}

// returns TRUE if the touch was handled
bool entity::on_touched( simple_collision::pending_collision& coll )
{
	return false;
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

void entity::apply_movement_jump()
{
	if( !simple.is_in_air )
	{
		add_impulse( { vec2( 0.0f, -1.0f ), 3.5f } );
	}
}

void entity::apply_movement_walk( vec2 delta, float speed )
{
	add_force( { delta * vec2::x_axis, 12.f } );
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
