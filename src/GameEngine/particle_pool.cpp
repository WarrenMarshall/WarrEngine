
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
	float pct_of_life, angle;
	w_color color;

	engine->opengl->push_matrix();
	{
		for( auto& iter : *particles )
		{
			if( iter.is_alive() )
			{
				pct_of_life = fabs( 1.0f - ( iter.life_span / iter.life_span_save ) );
				pct_of_life = w_clamp( pct_of_life, 0.0f, 1.0f );

				iter.t_color->get_value( pct_of_life, &color );

				float wk;
				iter.t_alpha->get_value( pct_of_life, &wk );
				color.a = wk;

				angle = iter.spin;

				{
					float scale;
					iter.t_scale->get_value( pct_of_life, &scale );

					engine->opengl->push_matrix();
					{
						engine->opengl->translate( iter.pos );
						engine->opengl->rotate( iter.spin );
						engine->opengl->scale( iter.base_scale * scale );

						// warren
						//engine->render->draw_sprite( iter.tex, color );
					}
					engine->opengl->pop_matrix();
				}
			}
		}
	}
	engine->opengl->pop_matrix();
}

void w_particle_pool::update()
{
	// update living particles in real time
	//
	// individual particles are updated in real time because we want
	// updates to their positions and rotations to be as smooth as possible.

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
