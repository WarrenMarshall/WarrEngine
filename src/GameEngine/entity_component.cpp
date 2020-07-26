
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_entity_component::w_entity_component() 
{
	anim_offset = w_random::getf();
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
}

// ----------------------------------------------------------------------------

ec_sprite::ec_sprite()
	: w_entity_component()
{
	type = component_type::sprite;
}

w_entity_component* ec_sprite::init( i_transform* parent_entity, a_subtexture* subtex )
{
	this->parent_entity = parent_entity;
	this->subtex = subtex;
	return this;
}

void ec_sprite::draw()
{
	if( is_dead() )
	{
		return;
	}

	RENDER->draw_sprite( subtex, w_rect( parent_entity->pos.x, parent_entity->pos.y ) );

	RENDER->draw_circle( parent_entity->pos, 16.0f );
}

// ----------------------------------------------------------------------------

ec_emitter::ec_emitter()
	: w_entity_component()
{
	type = component_type::emitter;
}

w_entity_component* ec_emitter::init( i_transform* parent_entity, const std::string& params_name )
{
	this->parent_entity = parent_entity;

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

	emitter->particle_pool->draw();
}

void ec_emitter::update()
{
	if( is_dead() )
	{
		return;
	}

	emitter->update();
	emitter->particle_pool->update();
	emitter->pos = parent_entity->pos;
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

ec_sound::ec_sound()
	: w_entity_component()
{
	type = component_type::sound;
}

w_entity_component* ec_sound::init( const std::string& snd_name )
{
	snd = engine->get_asset<a_sound>( snd_name );

	return this;

}

void ec_sound::draw()
{
	if( snd )
	{
		snd->play();
		snd = nullptr;
	}
}
