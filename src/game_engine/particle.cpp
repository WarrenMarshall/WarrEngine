
#include "master_pch.h"
#include "master_header.h"

w_particle::w_particle()
{
}

void w_particle::update()
{
	life_span -= engine->time->FTS_step_value_ms;

	if( !is_alive() )
	{
		return;
	}

	w_vec2 v = v_dir;
 	pos.x += v.x * ( velocity_per_sec * w_time::FTS_step_value_s );
	pos.y += v.y * ( velocity_per_sec * w_time::FTS_step_value_s );

	spin += spin_per_sec * w_time::FTS_step_value_s;
	spin = fmod( spin, 360.f );
}

constexpr bool w_particle::is_alive()
{
	return life_span > 0;
}
