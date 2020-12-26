
#include "master_pch.h"
#include "master_header.h"

void w_entity::update_from_physics()
{
	// entities with box2d components need their transforms
	// updated as per what the physics engine is reporting.
	//
	// NOTE : there should be only one "primary body" attached
	// to an entity so it is assumed that once we find and
	// process that one, we're done.

	for( auto& ec : components )
	{
		if( ec->type & (component_type::b2d_dynamic | component_type::b2d_kinematic ) )
		{
			auto* edb = static_cast<ec_b2d_body*>( ec.get() );
			if( edb->is_primary_body )
			{
				w_vec2 position = w_vec2( edb->body->GetPosition() ).from_b2d();
				float angle = edb->body->GetAngle();

				it_set( { position.x, position.y }, glm::degrees( angle ), scale );
				break;
			}
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

	for( auto x = 0; x < components.size(); ++x )
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
		OPENGL
			->push()
			->add_transform( component->pos, component->angle, component->scale );

		component->update();

		OPENGL
			->pop();
	}
}

void w_entity::draw()
{
	for( const auto& component : components )
	{
		OPENGL
			->push()
			->add_transform( component->pos, component->angle, component->scale );

		RENDER->push_depth_nudge();
		component->draw();

		OPENGL
			->pop();
	}
}

// immediately moves the entity and it's dynamic/kinematic bodies to a new position.

void w_entity::set_position_deep( w_vec2 pos, bool reset_velocity )
{
	// entity
	it_set_position( pos );

	// physics components
	auto ecs = get_components<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

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
	it_set_angle( angle );

	// physics components
	auto ecs = get_components<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

	for( auto ec : ecs )
	{
		ec->body->SetTransform( ec->body->GetTransform().p, glm::radians( angle ) );
		ec->body->SetAwake( true );
	}
}

void w_entity::remove_component( w_entity_component* ec )
{
	for( auto x = 0 ; x < components.size() ; ++x )
	{
		if( components[x].get() == ec )
		{
			components.erase( components.begin() + x );
			return;
		}
	}

	assert( false );	// the entity component wasn't found
}

ec_b2d_body* w_entity::phys_get_primary_body()
{
	for( auto& ec : components )
	{
		if( ec->type & ( component_type::b2d_dynamic | component_type::b2d_kinematic ) )
		{
			auto* edb = static_cast<ec_b2d_body*>( ec.get() );
			if( edb->is_primary_body )
			{
				return edb;
			}
		}
	}

	assert( false );	// no primary body found!
	return nullptr;
}

// friction : 0 - slide, 1 - stick
void w_entity::phys_set_friction( float friction )
{
	for( b2Fixture* fixture = phys_get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetFriction( friction );
	}
}

// restitution : 0 = no bounce, 1 = full bounce
void w_entity::phys_set_restitution( float restitution )
{
	for( b2Fixture* fixture = phys_get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetRestitution( restitution );
	}
}

// density : 0 = no density, 1 = full density
void w_entity::phys_set_density( float density )
{
	for( b2Fixture* fixture = phys_get_primary_body()->body->GetFixtureList(); fixture; fixture = fixture->GetNext() )
	{
		fixture->SetDensity( density );
	}
}

// entities are start to touch each other

void w_entity::phys_begin_contact( w_pending_collision& coll, w_entity* other )
{
}

// entities were touching but are not anymore

void w_entity::phys_end_contact( w_pending_collision& coll, w_entity* other )
{
}

bool w_entity::can_be_deleted()
{
	// still alive, can't delete
	if( ilc_is_alive() ) {
		return false;
	}

	if( ilc_update_count_death_delay ) {
		return false;
	}

	if( ilc_is_dying() ) {
		// entity is dying, but can't be deleted until all
		// components are dead

		for( const auto& iter : components ) {
			if( !iter->is_fully_dead() ) {
				return false;
			}
		}
	}

	// entity is fully dead, delete it
	return true;
}

void w_entity::set_tag( const char* tag )
{
	this->tag = tag;
}

void w_entity::set_collision( bitflags layer, bitflags collides_with )
{
	this->collision_layer = layer;
	this->collides_with = collides_with;
}

void w_entity::ilc_set( e_life_cycle life_cycle )
{
	i_life_cycle::ilc_set( life_cycle );

	if( !ilc_is_alive() )
	{
		collision_layer = 0;
		collides_with = 0;
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

void e_camera::set_follow_target( w_entity* entity_to_follow, e_follow_flags flags, float strength )
{
	follow.target = entity_to_follow;
	follow.flags = flags;
	follow.strength = strength;
	follow.pos = entity_to_follow->pos;

	set_position_deep( follow.target->pos, false );
}

void e_camera::set_follow_limits_x( w_vec2 limits )
{
	follow.limits_x = limits;
}

void e_camera::set_follow_limits_y( w_vec2 limits )
{
	follow.limits_y = limits;
}

void e_camera::update()
{
	w_entity::update();

	if( follow.target )
	{
		// position

		w_vec2 delta_pos = follow.target->pos - pos;

		if( follow.flags & follow_flags::xy_axis )
		{
			// interpolate towards follow target position
			follow.pos += (( follow.target->pos - follow.pos ) * follow.strength ) * w_time::FTS_step_value_s;

			// apply limits if we need to
			if( follow.limits_x.has_value() )
			{
				follow.pos.x = glm::clamp( follow.pos.x, follow.limits_x->l, follow.limits_x->r );
			}
			if( follow.limits_y.has_value() )
			{
				follow.pos.y = glm::clamp( follow.pos.y, follow.limits_y->t, follow.limits_y->b );
			}

			// if only following on a specific axis, remove the follow influence from the other
			if( !(follow.flags & follow_flags::x_axis) )
			{
				follow.pos.x = pos.x;
			}
			if( !( follow.flags & follow_flags::y_axis ) )
			{
				follow.pos.y = pos.y;
			}
		}

		set_position_deep( follow.pos, false );

		// angle

		if( follow.flags & follow_flags::angle )
		{
			set_angle_deep( follow.target->angle );
		}
	}
}
