
#include "master_pch.h"
#include "master_header.h"

w_particle_pool::w_particle_pool( int num_particles )
	: pool_sz( num_particles )
{
	particles.resize( pool_sz );
	data_ptr = particles.data();
	//log_msg( "Creating a particle pool with {} particles", num_particles );
}

w_particle* w_particle_pool::get_next_particle()
{
	// move the idx ahead by 1, wrapping around from the end to the start
	idx = ( idx + 1 ) % pool_sz;

	// return a pointer to the particle at that index.
	return data_ptr + idx;
}

void w_particle_pool::draw()
{
	RENDER
		->begin();

	w_particle* particle = data_ptr;
	for( auto p = 0 ; p < pool_sz ; ++p )
	{
		if( particle->is_alive() )
		{
			// lifetime
			float interp_life_span = RENDER->calc_interpolated_per_sec_value( particle->life_span, -engine->time->FTS_step_value_ms );

			float pct_of_life = glm::abs( 1.0f - ( interp_life_span / particle->life_span_save ) );
			pct_of_life = glm::clamp( pct_of_life, 0.0f, 1.0f );

			// color + alpha
			w_color color;
			particle->params->t_color->get_value( pct_of_life, &color );
			particle->params->t_alpha->get_value( pct_of_life, &color.a );

			// scale
			float scale;
			particle->params->t_scale->get_value( pct_of_life, &scale );

			// angle
			float interp_angle = RENDER->calc_interpolated_per_sec_value( particle->spin, particle->spin_per_sec );

			// position
			w_vec2 v = particle->v_dir;
			w_vec2 interp_pos(
				RENDER->calc_interpolated_per_sec_value( particle->pos.x, ( v.x * particle->velocity_per_sec ) ),
				RENDER->calc_interpolated_per_sec_value( particle->pos.y, ( v.y * particle->velocity_per_sec ) )
			);

			// the particle system issues a LOT of draw calls and these values are changing for every
			// particle. it doesn't make sense to go through the render_state stack calls to set these
			// so we just set them here directly.
			//
			// the call to RENDER->end() at the end of the function resets the render_state stacks so
			// there's no harm done here. it's just a faster way of telling the renderer what each
			// particle wants.

			auto rs = RENDER->rs_top();

			rs->color = color;
			rs->scale.x = particle->base_scale * scale;
			rs->scale.y = rs->scale.x;
			rs->angle = interp_angle;

			RENDER->draw_sprite( particle->texture, interp_pos );
		}

		particle++;
	}

	RENDER->end();
}

void w_particle_pool::update()
{
	num_alive = 0;

	w_particle* particle = data_ptr;
	for( auto p = 0 ; p < pool_sz ; ++p )
	{
		if( particle->is_alive() )
		{
			num_alive++;
			particle->update();
		}

		particle++;
	}
}
