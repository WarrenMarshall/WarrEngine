
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_entity_component::w_entity_component()
	: i_transform()
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
		set_life_cycle( e_lifecycle::dead );
	}
}

// ----------------------------------------------------------------------------

c_sprite::c_sprite()
	: w_entity_component()
{
	type = e_component_type::sprite;
}

w_entity_component* c_sprite::init( const std::string& subtexture_name )
{
	//img = std::make_unique<a_subtexture>( subtexture_name );
	return this;
}

w_entity_component* c_sprite::init( const std::string& subtexture_name, const w_rect& rc )
{
	//img = std::make_unique<a_subtexture>( image_name, rc );
	return this;
}

void c_sprite::draw()
{
	if( is_dead() )
	{
		return;
	}

	RENDER->draw_sprite( img );
}

// ----------------------------------------------------------------------------

c_emitter::c_emitter()
	: w_entity_component()
{
	type = e_component_type::emitter;
}

w_entity_component* c_emitter::init( i_transform* parent_entity, const std::string& params_name )
{
	this->parent_entity = parent_entity;

	emitter = std::make_unique<w_particle_emitter>();
	emitter->set_params( engine->get_asset<a_emitter_params>( params_name ) );
	emitter->parent_component = this;

	return this;
}

bool c_emitter::is_fully_dead()
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

void c_emitter::draw()
{
	if( is_dead() )
	{
		return;
	}

	MATRIX
		->push_identity();

	emitter->particle_pool->draw();

	MATRIX
		->pop();
}

void c_emitter::update()
{
	if( is_dead() )
	{
		return;
	}

	emitter->update();
	emitter->particle_pool->update();
	emitter->pos = parent_entity->pos;
}

void c_emitter::set_life_cycle( e_lifecycle lifecycle )
{
	i_lifecycle::set_life_cycle( lifecycle );

	if( is_dying() )
	{
		emitter->max_particles_alive = 0;
	}
}

void c_emitter::post_spawn()
{
	emitter->post_spawn();
}

// ----------------------------------------------------------------------------

c_sound::c_sound()
	: w_entity_component()
{
	type = e_component_type::sound;
}

w_entity_component* c_sound::init( const std::string& snd_name )
{
	snd = engine->get_asset<a_sound>( snd_name );

	return this;

}

void c_sound::draw()
{
	if( snd )
	{
		snd->play();
		snd = nullptr;
	}
}
