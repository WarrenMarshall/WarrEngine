
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

void effect::update()
{
}

// ----------------------------------------------------------------------------

timed_effect::timed_effect( time_ms duration )
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

te_transform_shake::te_transform_shake( time_ms duration, transform* tform, float strength )
	: timed_effect( duration ),
	tform( tform ), strength( strength )
{
}

void te_transform_shake::update()
{
	timed_effect::update();

	if( get_life_cycle()->is_alive() )
	{
		tform->pos.x += noise.get() * strength;
		tform->pos.y += noise.get() * strength;
	}
}

}
