
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

Entity_Simple_Force::Entity_Simple_Force( Vec2 normal, float_t strength )
	: strength( strength )
{
	this->normal = Vec2::normalize( normal );
}

// ----------------------------------------------------------------------------

Entity::Entity()
{
}

Entity::Entity( std::string debug_name )
{
#ifdef _DEBUG
	this->debug_name = debug_name;
#endif
}

void Entity::pre_update()
{
	life_cycle.pre_update();

	apply_forces();

	nodes.clear();
}

void Entity::update()
{
}

void Entity::pre_update_components()
{
	for( const auto& component : components )
	{
		scoped_opengl;
		g_engine->opengl_mgr.top_matrix->apply_transform( *component->get_transform() );

		component->pre_update();
	}
}

void Entity::update_components()
{
	remove_dead_components();

	for( const auto& component : components )
	{
		scoped_opengl;
		g_engine->opengl_mgr.top_matrix->apply_transform( *component->get_transform() );

		component->update();
	}
}

void Entity::post_update_components()
{
	for( const auto& component : components )
	{
		scoped_opengl;
		g_engine->opengl_mgr.top_matrix->apply_transform( *component->get_transform() );

		component->post_update();
	}

	simple_collision_ws_aabb = compute_ws_aabb();

#ifndef _FINAL_RELEASE

	if( g_engine->render.debug.is_entity_info_logging() )
	{

		auto tform = get_transform();
		if( debug_name.length() )
		{
			log( "{}", debug_name );
		}
		log( "P : {:.2f}, {:.2f}", tform->pos.x, tform->pos.y );
		log( "V : {:.8f}, {:.8f}", velocity.x, velocity.y );
		log( "QT Nodes : {}", nodes.size() );
		log( "---" );
	}
#endif
}

void Entity::post_update()
{
}

// forces are gradual and build up over time.
//
// this is good for moving a character or applying things like gravity or wind.
void Entity::add_force( const Entity_Simple_Force& force )
{
	pending_forces.emplace_back( force.normal, fixed_time_step::per_second( force.strength ) );
}

// impulses are immediate and applied at full strength.
//
// this is for things like jumping or bouncing off of things.
void Entity::add_impulse( const Entity_Simple_Force& force )
{
	pending_forces.push_back( force );
}

void Entity::compile_velocity()
{
	for( auto& f : pending_forces )
	{
		velocity += f.normal * f.strength;
	}

	velocity.x = simple.max_velocity_x.clamp_value( velocity.x );
	velocity.y = simple.max_velocity_y.clamp_value( velocity.y );

	pending_forces.clear();
}

void Entity::reflect_across( Vec2 normal )
{
	auto reflected_dir = Vec2::reflect_across_normal( velocity, normal.normalize() );

	if( !reflected_dir.is_zero() )
	{
		add_impulse( { reflected_dir, velocity.get_size() } );
		velocity = Vec2::zero;
	}
}

void Entity::apply_forces()
{
	if( simple.is_affected_by_gravity )
	{
		auto accel = 1.0f;

		// if entity is already falling downwards, accelerate the fall. this
		// makes jumping feel tighter.
		if( velocity.y > 0.f )
		{
			accel = 2.0f;
		}

		add_force( { Vec2::y_axis, simple_collision_gravity_default * accel } );
	}

	compile_velocity();

	// if the velocity has reached a point where it's so small we're just jittering, clear it out.
	if( simple.is_dynamic() )
	{
		if( glm::abs( velocity.y ) < 0.1f )
		{
			velocity.y = 0.f;
		}

		if( glm::abs( velocity.x ) < 0.1f )
		{
			velocity.x = 0.f;
		}
	}

	if( !velocity.is_zero() )
	{
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
			// only dampen the vertical velocity on the way up
			if( velocity.y < 0.0f )
			{
				simple.bounce_needs_dampening = false;
				velocity.y *= 0.5f;
			}
		}
	}
}

// ----------------------------------------------------------------------------

void Entity::draw()
{
	// #selection
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
		g_engine->opengl_mgr.top_matrix->apply_transform( component->get_pos(), component->get_angle(), component->get_scale() );

		Render::state->z += zdepth_nudge;

		scoped_render_state;

		Render::state->set_from_opt( rs_opt );
		Render::state->pick_id = pick_id;
		component->draw();
	}
}

// called whenever the transform for an entity is changed. if there are physics
// components attached, they are updated to match the entity transform.
//
// this is generally not done during gameplay, but more as a set up thing. if
// you have physics, let the physics engine control what the entity transform is
// via the regular update() cycle.

void Entity::update_box2d_components_to_match_transform()
{
	// ----------------------------------------------------------------------------
	// BOX2D

	if( has_component<Box2D_Physics_Component>() )
	{
		std::vector<Box2D_Physics_Body_Component*> ecs;
		get_components<Box2D_Physics_Body_Component, Box2D_Dynamic_Body_Component>( ecs );
		get_components<Box2D_Physics_Body_Component, Box2D_Kinematic_Body_Component>( ecs );

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

void Entity::update_transform_to_match_box2d_components()
{
	// ----------------------------------------------------------------------------
	// BOX2D

	// entities with physics components need their transforms
	// updated as per what the physics engine is reporting.
	//
	// NOTE : there should be only one "primary body" attached
	// to an entity so it is assumed that once we find and
	// process that one, we're done.

	if( auto ec = find_primary_box2d_body() ; ec )
	{
		Vec2 position = Vec2( ec->body->GetPosition() ).from_box2d();
		auto angle = ec->body->GetAngle();

		_tform.set_pos( { position.x, position.y } );
		_tform.set_angle( glm::degrees( angle ) );
	}
}

const war::Transform* Entity::get_transform()
{
	return &_tform;
}

Transform* Entity::set_pos_angle_scale( const Vec2& pos, const float_t angle, const float_t scale )
{
	_tform.set_pos( pos );
	_tform.set_angle( angle );
	_tform.set_scale( scale );

	update_box2d_components_to_match_transform();

	return &_tform;
}

Transform* Entity::set_angle( const float_t angle )
{
	_tform.set_angle( angle );
	update_box2d_components_to_match_transform();

	return &_tform;
}

Transform* Entity::set_scale( const float_t scale )
{
	_tform.set_scale( scale );

	// scaling an entity with physics components isn't encouraged as we
	// don't have a way to scale the physics components at the same time. so
	// collision will remain the original size which is likely not what you
	// want.
	assert( !has_component<Box2D_Physics_Component>() );

	return &_tform;
}

Vec2 Entity::get_pos()
{
	return get_transform()->pos;
}

float_t Entity::get_angle()
{
	return get_transform()->angle;
}

float_t Entity::get_scale()
{
	return get_transform()->scale;
}

Transform* Entity::set_pos( const Vec2& pos )
{
	_tform.set_pos( pos );

	update_box2d_components_to_match_transform();

	return &_tform;
}

void Entity::add_delta_pos( const Vec2& delta )
{
	_tform.add_pos( delta );

	// if there are entities stuck to this one, move them at the same time
	for( auto e : sticky_set )
	{
		e->add_delta_pos( delta );
	}

	update_box2d_components_to_match_transform();
}

void Entity::add_delta_angle( const float_t delta )
{
	_tform.add_angle( delta );

	update_box2d_components_to_match_transform();
}

void Entity::add_delta_scale( const float_t delta )
{
	_tform.add_scale( delta );


	// scaling an entity with physics components isn't encouraged as we
	// don't have a way to scale the physics components at the same time. so
	// collision will remain the original size which is likely not what you
	// want.
	assert( !has_component<Box2D_Physics_Component>() );
}

// entities are starting to collide

void Entity::on_box2d_collision_begin( box2d_physics::Pending_Collision& coll, Entity* touched_by )
{
}

// entities are no longer colliding

void Entity::on_box2d_collision_end( box2d_physics::Pending_Collision& coll, Entity* touched_by )
{
}

// returns TRUE if the collision was handled
bool Entity::on_collided( simple_collision::Pending_Collision& coll )
{
	return false;
}

// returns TRUE if the touch was handled
bool Entity::add_sensor_to_touch_list( Simple_Collision_Body* sensor )
{
	sensors_this_frame.insert( sensor );
	return false;
}

bool Entity::on_touching_begin( Simple_Collision_Body* sensor )
{
	if( sensor->is_sticky )
	{
		sensor->parent_entity->sticky_set.insert( this );
	}

	if( sensor->tag == H( "ground_sensor" ) )
	{
		sensor->parent_entity->simple.is_in_air = false;
	}

	return false;
}

bool Entity::on_touching( Simple_Collision_Body* sensor )
{
	return false;
}

bool Entity::on_touching_end( Simple_Collision_Body* sensor )
{
	if( sensor->is_sticky )
	{
		sensor->parent_entity->sticky_set.erase( this );
	}

	if( sensor->tag == H( "ground_sensor" ) )
	{
		sensor->parent_entity->simple.is_in_air = true;
	}

	return false;
}

bool Entity::can_be_deleted()
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

void Entity::set_tag( hash tag )
{
	this->tag = tag;
}

static int32_t w_entity_last_pick_id = 0;

void Entity::set_pickable()
{
	w_entity_last_pick_id++;
	pick_id = w_entity_last_pick_id;
}

void Entity::set_life_cycle( e_life_cycle lc )
{
	life_cycle.set( lc );

	if( auto ec = get_component<Box2D_Physics_Component>() ; ec and !life_cycle.is_alive() )
	{
		ec->clear_collision_flags();
	}

	for( const auto& iter : components )
	{
		iter->life_cycle.set( lc );
	}
}

void Entity::remove_dead_components()
{
	for( size_t x = 0; x < components.size(); ++x )
	{
		Entity_Component* ec = components[ x ].get();

		if( ec->is_fully_dead() )
		{
			components.erase( components.begin() + x );
			x--;
		}
	}
}

void Entity::apply_movement_jump()
{
	if( !simple.is_in_air )
	{
		//g_engine->find_asset<sound_asset>( "sfx_platformer_jump" )->play();
		add_impulse( { Vec2( 0.0f, -1.0f ), 3.5f } );
	}
}

void Entity::apply_movement_walk( Vec2 delta, float_t speed )
{
	auto dot = Vec2::dot( delta, Vec2::down );

	// if the delta is mostly aimed downwards, then don't apply the delta. this
	// allows the player to pull down on the stick without the entity sliding
	// left and right.

	if( dot > -0.85f )
	{
		add_force( { delta * Vec2::x_axis, 12.f } );
	}
}

// looks at the simple collision bodies for this entity and generates an all
// encompassing AABB in world space that represents the entire bounds of the
// entity.
//
// this doesn't care about the actual shape of the collider, it only cares about
// it's AABB in world space. all colliders compute their AABB automatically
// during their update.

Rect Entity::compute_ws_aabb() const
{
	auto scbs = get_components<Simple_Collision_Body, Simple_Collision_Body>();
	Bounding_Box bbox;

	for( const auto& ec : scbs )
	{
		if( !ec->is_sensor() )
		{
			bbox += ec->ws.aabb;
		}
	}

	return bbox.as_rect();
}

Box2D_Physics_Body_Component* Entity::find_primary_box2d_body() const
{
	std::vector<Box2D_Physics_Body_Component*> ecs;
	get_components<Box2D_Physics_Body_Component, Box2D_Dynamic_Body_Component>( ecs );
	get_components<Box2D_Physics_Body_Component, Box2D_Kinematic_Body_Component>( ecs );

	for( auto& ec : ecs )
	{
		if( ec->is_primary_body )
		{
			return ec;
		}
	}

	return nullptr;
}

// ----------------------------------------------------------------------------

void Entity_Transient::update()
{
	Entity::update();

	// once all of the components have died, the fx container entity can die.
	if( components.empty() )
	{
		set_life_cycle( e_life_cycle::dying );
	}
}

}
