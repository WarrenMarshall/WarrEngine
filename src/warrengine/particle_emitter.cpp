
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Particle_Emitter::set_params( Emitter_Parameters_Asset* params )
{
	this->params = params;

	// "one shot" emitters don't get throttled like regular emitters.
	// they spawn all of their particles, no matter what.
	if( params->is_one_shot )
	{
		max_particles_alive = (int32_t)( params->s_max_alive );
	}
	else
	{
		max_particles_alive = (int32_t)( params->s_spawn_rate * ( params->r_lifespan.end / 1000.f ) );
	}

	// if "s_max_alive" has been set in the params, that overrides any calculated value from above
	max_particles_alive = ( params->s_max_alive > 0.f ) ? (int32_t)params->s_max_alive : max_particles_alive;

	//log( "{} particles allocated in pool", max_particles_alive );

	pool = {};
	pool.init_to_size( max_particles_alive );
	pool.parent_emitter = this;
}

void Particle_Emitter::update()
{
	if( !parent_component->life_cycle.is_alive() )
	{
		return;
	}

	// accumulate the time step into our spawn count
	particles_to_spawn_accum += fixed_time_step::per_second( params->s_spawn_rate );

	// if we are below the max particle count, spawn more particles

	if( pool.num_alive >= max_particles_alive )
	{
		return;

	}

	int32_t particles_to_spawn = 0;

	if( params->is_one_shot )
	{
		particles_to_spawn_accum = 0.f;
		particles_to_spawn = (int32_t)( params->s_max_alive );
	}
	else
	{
		// strip off the fractional part of the accum to get the number to spawn
		particles_to_spawn = (int32_t)( glm::trunc( particles_to_spawn_accum ) );
	}

	if( particles_to_spawn )
	{
		// if 'particles_to_spawn' would go over the max limit, drop it down so
		// we will hit the max exactly.
		if( pool.num_alive + particles_to_spawn > max_particles_alive )
		{
			particles_to_spawn = max_particles_alive - pool.num_alive;
		}

		// subtract the int value from the accumulator, leaving the fractional value only
		particles_to_spawn_accum -= glm::trunc( particles_to_spawn_accum );

		while( particles_to_spawn )
		{
			spawn_particle();
			particles_to_spawn--;
		}
	}

	// a one-shot particle system spawns all of it's particles at once and then dies
	if( params->is_one_shot )
	{
		parent_component->life_cycle.set( e_life_cycle::dying );
	}
}

void Particle_Emitter::spawn_particle()
{
	// get a pointer to the next particle slot in the pool

	Particle* p = pool.get_next();

	// if that particle slot contains a living particle, don't stomp it. let the
	// spawn fail and the code will try again the next update() cycle.
	// ----------------------------------------------------------------------------
	// note : to get around this logic problem, we either need to:
	//
	// 1 - loop through all the particle slots to find a dead one
	// 2 - just check the next slot and abort if it's alive
	//
	// "1" could be pretty slow if you have a lot of slots and most are alive
	//
	// "2" wins here because there are 60 update cycles per second. you won't
	// notice if it fails to spawn a particle or two but you WOULD notice
	// particles getting stomped and vanishing off the screen as new ones are
	// spawned on top of them.

	if( p->is_alive )
	{
		return;
	}

	// ok, the slot is dead, so construct a new particle in this slot.

	*p = {};
	p->is_alive = true;

	// particle spawn locations are determined in stages. particles are
	// different from most things in that they are spawned and updated in world
	// space, independent of whatever is spawning them.

	// find a spawn location based on the particle spawner we are using relative
	// to the world origin (0,0). as if the spawner was sitting at the origin,
	// ignoring all parental/owner transforms...
	params->particle_spawner.find_spawn_pos_for_new_particle( p );

	// ...then apply the current transform (entity+component) to move the particle
	// position into world space
	auto save_particle_position_at_origin = p->pos;
	g_engine->opengl_mgr.top_matrix->transform_vec2( &p->pos );

	switch( (e_particle_spawn_dir_t)params->a_dir )
	{
		case e_particle_spawn_dir::inherit_from_owner:
		{
			auto a_dir = parent_component->parent_entity->get_angle();
			a_dir += params->r_dir_var.get_random_value();
			p->v_dir = Vec2::dir_from_angle( a_dir );
			break;
		}

		case e_particle_spawn_dir::away_from_owner:
		{
			p->v_dir = Vec2::normalize( save_particle_position_at_origin );
			break;
		}

		default:
		{
			auto a_dir = params->a_dir;
			a_dir += params->r_dir_var.get_random_value();
			p->v_dir = Vec2::dir_from_angle( a_dir );
			break;
		}
	}

	p->params = params;
	p->life_span_save = p->life_span = params->r_lifespan.get_random_value();
	p->velocity_per_sec = params->r_velocity_spawn.get_random_value();
	p->base_scale =
		params->r_scale_spawn.get_random_value()
		* parent_component->get_scale()
		* parent_component->parent_entity->get_scale();
	p->spin_per_sec = params->r_spin_per_sec.get_random_value();
	p->spin = params->r_spin_spawn.get_random_value();
	p->anim_offset = Random::getf();
}

// "warms up" a particle emitter by updating it for the maximum life span of one
// of it's particles. It does this in FTS chunks so it's pretty quick.
//
// This is good for not having all the torches turn on when you walk into a
// room, for example. They will look as tho they've been burning for awhile.

void Particle_Emitter::warm_up()
{
	float_t max_life_span = params->r_lifespan.end;

	while( max_life_span > 0 )
	{
		pool.update();
		update();

		max_life_span -= fixed_time_step::ms_per_step;
	}
}

}
