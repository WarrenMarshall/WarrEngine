
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void particle::update()
{
	life_span -= fixed_time_step::ms_per_step;

	pos += v_dir * ( fixed_time_step::per_second( velocity_per_sec ) );

	// #bug - this feels bad and looks janky - it's the right idea, wrong implementation
	// if you slow down time to 0.1, you can see it really start struggling
	gravity_accum += fixed_time_step::per_second( b2d_gravity_default );
	pos.y += gravity_accum * params->uses_gravity;

	spin += fixed_time_step::per_second( spin_per_sec );

	is_alive = ( life_span > 0 );
}

}
