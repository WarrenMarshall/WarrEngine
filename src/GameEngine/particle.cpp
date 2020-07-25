
#include "master_pch.h"
#include "master_header.h"

w_particle::w_particle() 
{
	init_to_new();
}

void w_particle::update()
{
	life_span -= engine->time->FTS_step_value_ms;

	if( !is_alive() )
	{
		return;
	}

	w_vec2 v = w_vec2::from_angle( a_dir );
	pos.x += v.x * ( velocity_per_sec * w_time::FTS_step_value_s );
	pos.y += v.y * ( velocity_per_sec * w_time::FTS_step_value_s );

	spin += spin_per_sec * w_time::FTS_step_value_s;
	spin = fmod( spin, 360.f );
}

bool w_particle::is_alive()
{
	return ( life_span > 0 );
}

void w_particle::init_to_new()
{
	life_span = 0.0f;
	life_span_save = 0.0f;
	a_dir = 0.0f;
	velocity_per_sec = 0.0f;
	t_color = nullptr;
	t_alpha = nullptr;
	pos = w_vec2::zero;
	t_scale = nullptr;
	base_scale = 0.0f;
	spin = 0.0f;
	spin_per_sec = 0.0f;
}
