
#include "master_pch.h"
#include "master_header.h"

void w_particle::update()
{
	life_span -= fixed_time_step::ms_per_step;
	pos += v_dir * ( velocity_per_sec * fixed_time_step::per_second_scaler );
	spin += spin_per_sec * fixed_time_step::per_second_scaler;
}

bool w_particle::is_alive() const
{
	return life_span > 0;
}
