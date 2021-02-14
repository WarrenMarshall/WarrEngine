
#include "master_pch.h"
#include "master_header.h"

w_particle_pool::w_particle_pool( int num_particles )
{
	particles.resize( num_particles );

	log_verbose( "Creating a particle pool with {} particles", num_particles );
}

w_particle* w_particle_pool::get_next_particle()
{
	auto sz = particles.size();

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

			// position
			interp_pos.x = RENDER->calc_interpolated_per_sec_value( particle.pos.x, ( particle.v_dir.x * particle.velocity_per_sec ) );
			interp_pos.y = RENDER->calc_interpolated_per_sec_value( particle.pos.y, ( particle.v_dir.y * particle.velocity_per_sec ) );

			*rs_ptr = {
				.angle = RENDER->calc_interpolated_per_sec_value( particle.spin, particle.spin_per_sec ),
				.color = color,
				.scale = particle.base_scale * scale
			};

			w_render::draw_sprite( particle.params->texture, interp_pos );
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
