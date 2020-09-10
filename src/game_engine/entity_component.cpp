

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
	if( is_alive() || life_timer )
	{
		return false;
	}

	return true;
}

void w_entity_component::update()
{
	// if a timer is being used, and it has elapsed, then this component is dead
	if( life_timer )
	{
		life_timer->update();

		if( life_timer->is_elapsed() )
		{
			life_timer = nullptr;
			set_life_cycle( life_cycle::dying );
		}
	}

	// if this component is trying to die AND it meets the requirements to
	// be fully dead, then mark it dead

	if( is_dying() && is_fully_dead() )
	{
		set_life_cycle( life_cycle::dead );
	}

	pos_interp = w_vec2::zero;
}

void w_entity_component::set_life_timer( int life_in_ms )
{
	assert( life_timer == nullptr );

	life_timer = std::make_unique<w_timer>( life_in_ms );
}

// ----------------------------------------------------------------------------

ec_sprite::ec_sprite( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
}

w_entity_component* ec_sprite::init( const std::string_view subtex_name )
{
	subtex = engine->get_asset<a_subtexture>( subtex_name );
	return this;
}

void ec_sprite::draw()
{
	if( is_dead() )
	{
		return;
	}

	w_vec2 pos_interp = w_vec2::multiply( parent_entity->physics_cache.forces, RENDER->frame_interpolate_pct );
	RENDER->draw_sprite( subtex, w_rect( pos_interp.x, pos_interp.y ) );
}

// ----------------------------------------------------------------------------

ec_emitter::ec_emitter( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
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

	// particles live in world space, so remove any entity and
	// component level transforms before drawing the particle pool

	MATRIX
		->push_identity();

	emitter->particle_pool->draw();

	MATRIX
		->pop();

}

void ec_emitter::update()
{
	w_entity_component::update();

	if( is_dead() )
	{
		return;
	}

	emitter->update();
	emitter->particle_pool->update();
}

void ec_emitter::set_life_cycle( e_life_cycle life_cycle )
{
	i_lifecycle::set_life_cycle( life_cycle );

	if( is_dying() )
	{
		// setting the max to zero will cause the emitter to stop spawning new particles.
		emitter->max_particles_alive = 0;
	}
}

void ec_emitter::post_spawn()
{
	emitter->post_spawn();
}

// ----------------------------------------------------------------------------

ec_sound::ec_sound( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
}

w_entity_component* ec_sound::init( const std::string_view snd_name )
{
	snd = engine->get_asset<a_sound>( snd_name );

	return this;
}

void ec_sound::draw()
{
	if( snd )
	{
		snd->play();
	}
	snd = nullptr;

	set_life_cycle( life_cycle::dying );
}

// ----------------------------------------------------------------------------

ec_collider::ec_collider( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
}

void ec_collider::push_outside( const c2Manifold& hit, float extra_distance )
{
	w_vec2 push_delta = w_vec2::multiply( w_vec2( hit.n.x, hit.n.y ), -1.0f * ( hit.depths[ 0 ] + extra_distance ) );
	parent_entity->set_pos( parent_entity->pos.add( push_delta ) );
}

w_entity_component* ec_collider::init_as_circle( float radius )
{
	c2type = C2_TYPE_CIRCLE;

	this->radius = radius;

	return this;
}

w_entity_component* ec_collider::init_as_box( w_rect box )
{
	c2type = C2_TYPE_AABB;

	this->box = box;

	return this;
}

variant_collider_types ec_collider::get_collider()
{
	if( c2type == C2_TYPE_CIRCLE )
	{
		return c2Circle(
			{
				{ parent_entity->physics_cache.ending_pos.x, parent_entity->physics_cache.ending_pos.y },
				radius * parent_entity->scale
			}
		);
	}
	else if( c2type == C2_TYPE_AABB )
	{
		return (c2AABB) w_rect(
			{ parent_entity->physics_cache.ending_pos.x + box.x, parent_entity->physics_cache.ending_pos.y + box.y, box.w, box.h }
		);
	}

	assert( false );	// unknown collider type
	return {};
}

void ec_collider::draw()
{
	if( parent_entity->debug_draw_collision )
	{
		RENDER->push_rgb( w_color::green );

		if( c2type == C2_TYPE_CIRCLE )
		{
			RENDER->draw_circle( w_vec2::zero, radius );
		}
		else if( c2type == C2_TYPE_AABB )
		{
			RENDER->draw_rectangle( box );
		}

		RENDER->pop_rgb();
	}
}

// ----------------------------------------------------------------------------

ec_force_constant::ec_force_constant( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
}

ec_force_constant* ec_force_constant::init( float angle, float strength )
{
	this->angle = angle;
	this->strength = strength;

	return this;
}

// ----------------------------------------------------------------------------

ec_force_decay::ec_force_decay( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
}

ec_force_decay* ec_force_decay::init( float angle, float strength, float lifetime_in_ms )
{
	this->angle = angle;
	this->_strength = this->strength = strength;
	this->_lifetime_in_ms = this->lifetime_in_ms = lifetime_in_ms;

	return this;
}

void ec_force_decay::update()
{
	w_entity_component::update();

	lifetime_in_ms -= w_time::FTS_step_value_ms;
	lifetime_in_ms = w_max( lifetime_in_ms, 0.0f );
	float pct = lifetime_in_ms / _lifetime_in_ms;

	if( fequals( pct, 0.0f ) )
	{
		strength = 0.0f;
		set_life_cycle( life_cycle::dying );
	}
	else
	{
		strength = _strength * pct;
	}
}

// ----------------------------------------------------------------------------

ec_force_dir_accum::ec_force_dir_accum( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
}

ec_force_dir_accum* ec_force_dir_accum::init( float angle, float strength_per_sec, float strength_max )
{
	this->angle = angle;
	this->strength_per_sec = this->strength = strength_per_sec;
	this->strength_max = strength_max;

	return this;
}

void ec_force_dir_accum::add_impulse()
{
	strength += (strength_per_sec * 2.0f) * engine->time->FTS_step_value_s;
	strength = w_min( strength, strength_max );
}

void ec_force_dir_accum::decay()
{
	strength -= strength_per_sec * engine->time->FTS_step_value_s;
	strength = w_max( strength, 0.0f );
}
