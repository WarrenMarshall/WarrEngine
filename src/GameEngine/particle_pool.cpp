
#include "master_pch.h"
#include "master_header.h"

w_particle_pool::w_particle_pool( int num_particles )
{
	particles = std::make_unique<std::vector<w_particle>>( num_particles );

	//log_msg( "%s : creating particles pool with %d particles", __FUNCTION__, num_particles );
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
	MATRIX->push();

	for( auto& iter : *particles )
	{
		if( iter.is_alive() )
		{
			float interp_life_span = RENDER->calc_interpolated_per_sec_value( iter.life_span, -engine->time->FTS_step_value_ms );

			float pct_of_life = fabs( 1.0f - ( interp_life_span / iter.life_span_save ) );
			pct_of_life = w_clamp( pct_of_life, 0.0f, 1.0f );

			// color + alpha
			w_color color;
			iter.t_color->get_value( pct_of_life, &color );
			iter.t_alpha->get_value( pct_of_life, &color.a );

			// scale
			float scale;
			iter.t_scale->get_value( pct_of_life, &scale );

			float interp_angle = RENDER->calc_interpolated_per_sec_value( iter.spin, iter.spin_per_sec );

			w_vec2 v = w_vec2::from_angle( iter.a_dir );
			w_vec2 iterp_pos(
				RENDER->calc_interpolated_per_sec_value( iter.pos.x, ( v.x * iter.velocity_per_sec ) ),
				RENDER->calc_interpolated_per_sec_value( iter.pos.y, ( v.y * iter.velocity_per_sec ) )
			);

			MATRIX
				->push()
				->add_transform( iterp_pos, interp_angle, iter.base_scale * scale );

			RENDER
				->begin()
				->push_color( color )
				->draw_sprite( iter.tex->get_subtexture() )
				->end();

			MATRIX->pop();
		}
	}
	MATRIX->pop();
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
