
#include "master_pch.h"
#include "master_header.h"

w_time::w_time()
{
	init();
}

void w_time::init()
{
	prev_frame_ms = get_time_ms();
}

// called at the start of each frame to update the internal time keeping

void w_time::update()
{
	int time_now_ms = get_time_ms();

	delta_ms = time_now_ms - prev_frame_ms;
	delta_ms = static_cast<int>( delta_ms * dilation );

	prev_frame_ms = time_now_ms;

	if( engine->is_paused )
	{
		delta_ms = 0;
	}

	fts_accum_ms += delta_ms;
}

// returns the current time in ms
//
// so 1 second would be returned as 1000 ticks

int w_time::get_time_ms() const
{
	return static_cast<int>( (float)glfwGetTime() * 1000.0f );
}
