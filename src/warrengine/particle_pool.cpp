
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void particle_pool::draw()
{

	for( const auto& particle : _objects )
	{
		if( !particle.is_alive )
		{
			continue;
		}

		// lifetime
		float interp_life_span = g_engine->renderer.calc_interpolated_per_sec_value( particle.life_span, -fixed_time_step::ms_per_step );
		float pct_of_life = glm::abs( 1.f - ( interp_life_span / particle.life_span_save ) );

		// color + alpha
		color color;
		particle.params->t_color.get_value( pct_of_life, &color );
		particle.params->t_alpha.get_value( pct_of_life, &color.a );

		// scale
		float scale;
		particle.params->t_scale.get_value( pct_of_life, &scale );


		// render state
		render::state->angle = g_engine->renderer.calc_interpolated_per_sec_value( particle.spin, particle.spin_per_sec );
		render::state->color = color;
		render::state->scale = particle.base_scale * scale;
		render::state->anim_offset = particle.anim_offset;

		// position
		vec2 interp_pos = {
			g_engine->renderer.calc_interpolated_per_sec_value( particle.pos.x, ( particle.v_dir.x * particle.velocity_per_sec ) ),
			g_engine->renderer.calc_interpolated_per_sec_value( particle.pos.y, ( particle.v_dir.y * particle.velocity_per_sec ) )
		};

		render::draw_sprite( particle.params->texture, interp_pos );
	}
}

void particle_pool::update()
{
	num_alive = 0;

	for( auto& particle : _objects )
	{
		if( particle.is_alive )
		{
			particle.update();
			num_alive++;
		}
	}
}

}
