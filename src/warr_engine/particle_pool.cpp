
#include "master_pch.h"
#include "master_header.h"

w_particle_pool::w_particle_pool( int num_particles )
{
	particles.resize( num_particles );

	log_verbose( "Creating a particle pool with {} particles", num_particles );
}

w_particle* w_particle_pool::get_next_particle()
{
	int sz = (int)particles.size();

	// move the idx ahead by 1, wrapping around from the end to the start

	idx++;

	if( idx >= sz )
	{
		idx -= sz;
	}

	// return a pointer to the particle at that index.
	return &( particles[ idx ] );
}

void w_particle_pool::draw()
{
	w_color color;
	float scale;
	float interp_life_span;
	float pct_of_life;
	w_vec2 interp_pos;

	for( const auto& particle : particles )
	{
		if( particle.is_alive )
		{
			// lifetime
			interp_life_span = RENDER->calc_interpolated_per_sec_value( particle.life_span, -fixed_time_step::ms_per_step );
			pct_of_life = glm::abs( 1.0f - ( interp_life_span / particle.life_span_save ) );

			// color + alpha
			particle.params->t_color.get_value( pct_of_life, &color );
			particle.params->t_alpha.get_value( pct_of_life, &color.a );

			// scale
			particle.params->t_scale.get_value( pct_of_life, &scale );

			// angle
			RS->angle = RENDER->calc_interpolated_per_sec_value( particle.spin, particle.spin_per_sec );

			// position
			interp_pos.x = RENDER->calc_interpolated_per_sec_value( particle.pos.x, ( particle.v_dir.x * particle.velocity_per_sec ) );
			interp_pos.y = RENDER->calc_interpolated_per_sec_value( particle.pos.y, ( particle.v_dir.y * particle.velocity_per_sec ) );

			// the particle system issues a LOT of draw calls and these values
			// are changing for every particle. it doesn't make sense to go
			// through the render_state stack calls to set these so we just set
			// them here directly.
			//
			// the call to RENDER->end() at the end of the function resets the
			// render_state stacks so there's no harm done here. it's just a
			// faster way of telling the renderer what each particle wants.

			RS->color = color;
			RS->scale = particle.base_scale * scale;

			RENDER->draw_sprite( particle.params->texture, interp_pos );
		}
	}
}

void w_particle_pool::update()
{
	num_alive = 0;

	for( auto& particle : particles )
	{
		if( particle.is_alive )
		{
			num_alive++;
			particle.update();
		}
	}
}
