

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
	// #todo - this reads weird, can it be simplified?
	if( is_dying() && is_fully_dead() )
	{
		set_life_cycle( lifecycle::dead );
	}

	pos_interp = w_vec2::zero;

	active_time_remaining_ms -= w_time::FTS_step_value_ms;
	active_time_remaining_ms = w_max( active_time_remaining_ms, 0.0f );
}

bool w_entity_component::is_active()
{
	return active || ( active_time_remaining_ms > 0.0f );
}

// ----------------------------------------------------------------------------

ec_sprite::ec_sprite( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type = component_type::sprite;
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
	if( is_dead() || !is_active() )
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

	if( is_dead() || !is_active() )
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

ec_sound::ec_sound( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type = component_type::sound;
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

	set_life_cycle( lifecycle::dying );
}

// ----------------------------------------------------------------------------

ec_collider::ec_collider( w_entity* parent_entity )
	: w_entity_component( parent_entity )
{
	type = component_type::collider;
}

void ec_collider::push_outside( const c2Manifold& hit )
{
	w_vec2 push_delta = w_vec2::multiply( w_vec2( hit.n.x, hit.n.y ), -1.0f * ( hit.depths[ 0 ] + 0.5f ) );
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
#if 1
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
#endif
}
