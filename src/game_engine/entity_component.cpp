

#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_entity_component::w_entity_component( w_entity* parent_entity )
	: parent_entity( parent_entity )
{
	generic_offset = w_random::getf();
}

/*
	each component can implement it's own criteria here for
	whether or not it is fully dead or not.

	components can sometimes have more work left to do before
	their owning entities can be safely deleted.

	reasons to return false from here might include:

	- you are emitting particles and need to wait until they are all faded
	  out before letting yourself get deleted.
	- you are waiting for a sound effect to finish playing
	- etc
*/
bool w_entity_component::is_fully_dead()
{
	if( is_alive() )
	{
		return false;
	}

	return true;
}

void w_entity_component::update()
{
	if( is_dying() && is_fully_dead() )
	{
		set_life_cycle( lifecycle::dead );
	}

	pos_interp = w_vec2::zero;
}

// ----------------------------------------------------------------------------

ec_sprite::ec_sprite( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type = component_type::sprite;
}

w_entity_component* ec_sprite::init( const std::string_view subtex_name )
{
	this->subtex = engine->get_asset<a_subtexture>( subtex_name );
	return this;
}

void ec_sprite::draw()
{
	if( is_dead() )
	{
		return;
	}

	w_vec2 pos_interp = w_vec2::multiply( parent_entity->cache.forces, RENDER->frame_interpolate_pct );
	RENDER->draw_sprite( subtex, w_rect( pos_interp.x, pos_interp.y ) );
}

// ----------------------------------------------------------------------------

ec_emitter::ec_emitter( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type = component_type::emitter;
}

w_entity_component* ec_emitter::init( const std::string_view params_name )
{
	emitter = std::make_unique<w_particle_emitter>();
	emitter->set_params( engine->get_asset<a_emitter_params>( params_name ) );
	emitter->parent_component = this;

	return this;
}

bool ec_emitter::is_fully_dead()
{
	if( w_entity_component::is_fully_dead() )
	{
		if( is_dying() && emitter->particle_pool->num_particles_alive == 0 )
		{
			return true;
		}
	}

	return false;
}

void ec_emitter::draw()
{
	if( is_dead() )
	{
		return;
	}

	// particles live in world space, so remove any transforms before
	// drawing the particle pool
	//
	MATRIX
		->push()
		->translate( w_vec2::multiply( pos, -1.0f ) )
		->translate( w_vec2::multiply( parent_entity->pos, -1.0f ) );

	emitter->particle_pool->draw();

	MATRIX
		->pop();

}

void ec_emitter::update()
{
	if( is_dead() )
	{
		return;
	}

	emitter->update();
	emitter->particle_pool->update();
}

void ec_emitter::set_life_cycle( e_lifecycle lifecycle )
{
	i_lifecycle::set_life_cycle( lifecycle );

	if( is_dying() )
	{
		emitter->max_particles_alive = 0;
	}
}

void ec_emitter::post_spawn()
{
	emitter->post_spawn();
}

// ----------------------------------------------------------------------------

ec_collider::ec_collider( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type = component_type::collider;
}

w_entity_component* ec_collider::init()
{
	return this;

}

void ec_collider::draw()
{
}
