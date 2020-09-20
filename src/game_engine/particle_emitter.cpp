
#include "master_pch.h"
#include "master_header.h"

w_particle_emitter::~w_particle_emitter()
{
}

void w_particle_emitter::set_params( a_emitter_params* params )
{
	this->params = params;

	// "one shot" emitters don't get throttled like regular emitters.
	// they spawn all of their particles, no matter what.
	if( params->b_one_shot )
	{
		max_particles_alive = static_cast<int>( params->s_max_spawn_per_sec );
	}
	else
	{
		max_particles_alive = static_cast<int>( params->s_max_spawn_per_sec * ( params->r_lifespan.max / 1000.f ) );
	}

	particle_pool = std::make_unique<w_particle_pool>( max_particles_alive );
}

void w_particle_emitter::post_spawn()
{
	if( params->b_needs_warm_up )
	{
		warm_up();
	}
}

void w_particle_emitter::update()
{
	if( !parent_component->is_alive() )
	{
		return;
	}

	// if we are below the max particle count, spawn more particles

	if( particle_pool->num_particles_alive < max_particles_alive )
	{
		int particles_to_spawn = 0;

		if( params->b_one_shot )
		{
			particles_to_spawn_accum = 0.0f;
			particles_to_spawn = static_cast<int>( params->s_max_spawn_per_sec );
		}
		else
		{
			// accumulate the time step into our spawn count
			particles_to_spawn_accum += params->s_max_spawn_per_sec * w_time::FTS_step_value_s;

			// strip off the fractional part of the accum to get the number to spawn
			particles_to_spawn = static_cast<int>( particles_to_spawn_accum );
		}

		if( particles_to_spawn )
		{
			// if 'particles_to_spawn' would go over the max limit, drop it down so
			// we will hit the max exactly.
			if( particle_pool->num_particles_alive + particles_to_spawn > max_particles_alive )
			{
				particles_to_spawn = max_particles_alive - particle_pool->num_particles_alive;
			}

			assert( particles_to_spawn > 0 );

			//log_msg( "Spawning {} particles", particles_to_spawn );
			while( particles_to_spawn )
			{
				spawn_particle();
				particles_to_spawn--;
			}

			// subtract the int value from the accumulator, leaving the fractional value only
			particles_to_spawn_accum -= static_cast<int>( particles_to_spawn_accum );
		}
	}

	// a one-shot particle system spawns all of it's particles at once and then dies
	if( params->b_one_shot )
	{
		parent_component->set_life_cycle( life_cycle::dying );
	}
}

void w_particle_emitter::spawn_particle()
{
	w_particle* p = particle_pool->get_next_particle();
	p->init_to_new();

	params->particle_spawner->find_spawn_pos_for_new_particle( p );
	MATRIX->top()->transform_vec2( p->pos );

	p->tex = params->tex;

	switch( static_cast<e_particle_spawn_dir>( params->a_dir ) )
	{
		case particle_spawn_dir::inherit_from_parent:
		{
			assert( parent_component );
			assert( parent_component->parent_entity );

			p->a_dir = parent_component->parent_entity->angle_facing;
			p->a_dir += params->r_dir_var.get_value();
			p->v_dir = w_vec2::from_angle( p->a_dir );
			p->a_dir = -999;
		}
		break;

		case particle_spawn_dir::away_from_center:
		{
			assert( parent_component );
			assert( parent_component->parent_entity );

			p->v_dir = ( p->pos - parent_component->parent_entity->pos ).normalize();
			p->a_dir = -999;
		}
		break;

		default:
		{
			p->a_dir = params->a_dir;
			p->a_dir += params->r_dir_var.get_value();
			p->v_dir = w_vec2::from_angle( p->a_dir );
			p->a_dir = -999;
		}
		break;
	}

	p->params = params;
	p->life_span_save = p->life_span = params->r_lifespan.get_value();
	p->velocity_per_sec = params->r_velocity_spawn.get_value();
	p->base_scale = params->r_scale_spawn.get_value();
	p->spin_per_sec = params->r_spin_per_sec.get_value();
	p->spin = params->r_spin_spawn.get_value();
}

/*
	"warms up" a particle emitter by updating it for the maximum life span of one of it's
	particles. It does this in 1000ms chunks so it's pretty quick.

	This is good for not having all the torches turn on when you walk into a room, for example.
	They will look as tho they've been burning for awhile.
*/
void w_particle_emitter::warm_up()
{
	// #todo - this needs to be change to warm up in fixed time steps
	/*
	float max_life_span = params->r_lifespan.max;

	engine->time->delta_ms = 1000;
	engine->time->delta_s = engine->time->delta_ms / 1000.f;
	engine->time->fts_accum_ms = 0;

	while( max_life_span > 0 )
	{
		engine->time->fts_accum_ms += engine->time->delta_ms;

		while( engine->time->fts_accum_ms >= w_time::FTS_step_value_ms )
		{
			engine->time->fts_accum_ms -= w_time::FTS_step_value_ms;
			update();
		}

		particle_pool->update();

		max_life_span -= engine->time->delta_ms;
	}

	engine->time->prev_frame_ms = engine->time->get_ticks();
	*/
}
