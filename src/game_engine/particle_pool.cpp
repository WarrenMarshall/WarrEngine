
#include "master_pch.h"
#include "master_header.h"

w_particle_pool::w_particle_pool( int num_particles )
{
	// #todo - can't this just be a block of memory we allocate? why incur the overhead of the std::vector?
	particles = std::make_unique<std::vector<w_particle>>( num_particles );

	//log_msg( "Creating a particle pool with {} particles", num_particles );
}

w_particle* w_particle_pool::get_next_particle()
{
	// move the idx ahead by 1, wrapping around from the end to the start
	idx++;
	idx %= particles->size();

	// return a pointer to the particle at that index.
	return &( *particles )[idx];
}

void w_particle_pool::draw()
{
	for( auto& iter : *particles )
	{
		if( iter.is_alive() )
		{
			float interp_life_span = RENDER->calc_interpolated_per_sec_value( iter.life_span, -engine->time->FTS_step_value_ms );

			float pct_of_life = fabs( 1.0f - ( interp_life_span / iter.life_span_save ) );
			pct_of_life = w_clamp( pct_of_life, 0.0f, 1.0f );

			// color + alpha
			w_color color;
			iter.params->t_color->get_value( pct_of_life, &color );
			iter.params->t_alpha->get_value( pct_of_life, &color.a );

			// scale
			float scale;
			iter.params->t_scale->get_value( pct_of_life, &scale );

			float interp_angle = RENDER->calc_interpolated_per_sec_value( iter.spin, iter.spin_per_sec );

			//w_vec2 v = w_vec2::from_angle( iter.a_dir );
			w_vec2 v = iter.v_dir;
			w_vec2 interp_pos(
				RENDER->calc_interpolated_per_sec_value( iter.pos.x, ( v.x * iter.velocity_per_sec ) ),
				RENDER->calc_interpolated_per_sec_value( iter.pos.y, ( v.y * iter.velocity_per_sec ) )
			);

			// #optimization	- bypassing the render state stacks would make this more performant
			//					- just set the color/alpha directly, and skip the push/pop for every particle
			RENDER
				->begin()
				->push_rgba( color )
				->push_scale( iter.base_scale * scale )
				->push_angle( interp_angle )
				->push_depth_nudge( 250 )	// kind of nudge the particles above whatever is rendering at the moment
				->draw_sprite( iter.tex->get_subtexture(), w_rect( interp_pos.x, interp_pos.y ) )
				->end();
		}
	}
}

void w_particle_pool::update()
{
	num_particles_alive = 0;
	for( auto& iter : *particles )
	{
		if( iter.is_alive() )
		{
			num_particles_alive++;
			iter.update();
		}
	}
}
