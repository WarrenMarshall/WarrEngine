
#include "master_pch.h"
#include "master_header.h"

void w_particle::update()
{
	life_span -= fixed_time_step::ms_per_step;

	if( !is_alive() )
	{
		return;
	}

	w_vec2 v = v_dir;
	pos.x += v.x * ( velocity_per_sec * fixed_time_step::per_second_scaler );
	pos.y += v.y * ( velocity_per_sec * fixed_time_step::per_second_scaler );

	spin += spin_per_sec * fixed_time_step::per_second_scaler;
	spin = fmod( spin, 360.f );
}

bool w_particle::is_alive() const
{
	return life_span > 0;
}
