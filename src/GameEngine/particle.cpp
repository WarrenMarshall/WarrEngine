
#include "master_pch.h"
#include "master_header.h"

w_particle::w_particle()
	: i_transform()
{
	init_to_new();
}

void w_particle::update()
{
	life_span -= engine->time->FTS_step_value_ms;

	if( life_span < 0 )
	{
		return;
	}

	w_vec2 v = w_vec2::from_angle( a_dir );
	pos.x += (v.x * velocity_per_sec) * engine->time->delta_s;
	pos.y += (v.y * velocity_per_sec) * engine->time->delta_s;

	spin += spin_per_sec * engine->time->delta_s;
}

bool w_particle::is_alive()
{
	return ( life_span > 0 );
}
