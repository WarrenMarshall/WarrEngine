
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

effect::effect( bool should_restore_state )
	: should_restore_state( should_restore_state )
{
}

void effect::update()
{
}

// ----------------------------------------------------------------------------

timed_effect::timed_effect( bool should_restore_state, time_ms duration )
	: effect( should_restore_state )
{
	time_finish = g_engine->time.current_frame_ms + duration;
}

// ----------------------------------------------------------------------------

void timed_effect::update()
{
	effect::update();

	if( g_engine->time.current_frame_ms > time_finish )
	{
		set_life_cycle( life_cycle::dying );
	}
}

// ----------------------------------------------------------------------------

te_transform_shake_angle::te_transform_shake_angle( bool should_restore_state, time_ms duration, transform* tform, float strength )
	: timed_effect( should_restore_state, duration ),
	tform( tform ), strength( strength )
{
	saved_state.angle = tform->angle;
}

void te_transform_shake_angle::update()
{
	timed_effect::update();

	if( get_life_cycle()->is_alive() )
	{
		//tform->pos.x += noise.get() * strength;
		//tform->pos.y += noise.get() * strength;
		tform->angle = saved_state.angle + ( noise.get() * strength );
	}
	else
	{
		if( should_restore_state )
		{
			tform->angle = saved_state.angle;
		}
	}
}

}
