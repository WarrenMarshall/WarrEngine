
#include "master_pch.h"
#include "master_header.h"

void w_particle_emitter::set_params( a_emitter_params* params )
{
	this->params = params;

	// "one shot" emitters don't get throttled like regular emitters.
	// they spawn all of their particles, no matter what.
	if( params->is_one_shot )
	{
		max_particles_alive = static_cast<int>( params->s_max_spawn_per_sec );
	}
	else
	{
		max_particles_alive = static_cast<int>( params->s_max_spawn_per_sec * ( params->r_lifespan.end / 1000.f ) );
	}

	particle_pool = std::make_unique<w_particle_pool>( max_particles_alive );
	particle_pool->parent_emitter = this;
}

void w_particle_emitter::update()
{
	if( !parent_component->ilc_is_alive() )
	{
		return;
	}

	// if we are below the max particle count, spawn more particles

	if( particle_pool->num_alive < max_particles_alive )
	{
		int particles_to_spawn = 0;

		if( params->is_one_shot )
		{
			particles_to_spawn_accum = 0.0f;
			particles_to_spawn = static_cast<int>( params->s_max_spawn_per_sec );
		}
		else
		{
			// accumulate the time step into our spawn count
			particles_to_spawn_accum += params->s_max_spawn_per_sec * fixed_time_step::per_second_scaler;

			// strip off the fractional part of the accum to get the number to spawn
			particles_to_spawn = static_cast<int>( glm::trunc( particles_to_spawn_accum ) );
		}

		if( particles_to_spawn )
		{
			// if 'particles_to_spawn' would go over the max limit, drop it down so
			// we will hit the max exactly.
			if( particle_pool->num_alive + particles_to_spawn > max_particles_alive )
			{
				particles_to_spawn = max_particles_alive - particle_pool->num_alive;
			}

			assert( particles_to_spawn > 0 );

			// subtract the int value from the accumulator, leaving the fractional value only
			particles_to_spawn_accum -= particles_to_spawn;

			while( particles_to_spawn )
			{
				spawn_particle();
				particles_to_spawn--;
			}
		}
	}

	// a one-shot particle system spawns all of it's particles at once and then dies
	if( params->is_one_shot )
	{
		parent_component->ilc_set( life_cycle::dying );
	}
}

void w_particle_emitter::spawn_particle()
{
	w_particle* p = particle_pool->get_next_particle();
	//new( p ) w_particle();
	*p = {};

	// particle spawn locations are determined in stages. particles are different
	// from most things in that they are spawned and updated in world space, independent
	// of whatever is spawning them.

	// 1. find a spawn location based on the particle spawner we are using
	//    relative to the world origin
	params->particle_spawner->find_spawn_pos_for_new_particle( p );

	// 2. apply the current transform (entity+component) to move the
	//    particle position into world space
	p->pos = OPENGL->top()->transform_vec2( p->pos );

	switch( static_cast<e_particle_spawn_dir>( params->a_dir ) )
	{
		case particle_spawn_dir::inherit_from_parent:
		{
			assert( parent_component );
			assert( parent_component->parent_entity );

			auto a_dir = parent_component->parent_entity->get_tform()->angle;
			a_dir += params->r_dir_var.get_value();
			p->v_dir = w_vec2::dir_from_angle( a_dir );
		}
		break;

		case particle_spawn_dir::away_from_center:
		{
			assert( parent_component );
			assert( parent_component->parent_entity );

			p->v_dir = ( p->pos - parent_component->parent_entity->get_tform()->pos ).normalize();
		}
		break;

		default:
		{
			auto a_dir = params->a_dir;
			a_dir += params->r_dir_var.get_value();
			p->v_dir = w_vec2::dir_from_angle( a_dir );
		}
		break;
	}

	p->params = params;
	p->life_span_save = p->life_span = params->r_lifespan.get_value();
	p->velocity_per_sec = params->r_velocity_spawn.get_value();
	p->base_scale = params->r_scale_spawn.get_value() * parent_component->parent_entity->get_tform()->scale;
	p->spin_per_sec = params->r_spin_per_sec.get_value();
	p->spin = params->r_spin_spawn.get_value();
}

// "warms up" a particle emitter by updating it for the maximum life span of one
// of it's particles. It does this in 1000ms chunks so it's pretty quick.
//
// This is good for not having all the torches turn on when you walk into a
// room, for example. They will look as tho they've been burning for awhile.

void w_particle_emitter::warm_up()
{
	float max_life_span = params->r_lifespan.end;

	while( max_life_span > 0 )
	{
		update();
		particle_pool->update();

		max_life_span -= fixed_time_step::ms_per_step;
	}
}
