
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
	// get current time
	time_now_ms = get_time_ms();

	// compute the delta since the last frame
	delta_ms = time_now_ms - prev_frame_ms;

	if( engine->is_paused )
	{
		// if the engine has paused, pretend like no time has passed
		delta_ms = 0;
	}
	else
	{
		delta_ms = (time_ms)( delta_ms * dilation );
		fts_accum_ms += delta_ms;
	}

	// save the current time as the previous frame time
	prev_frame_ms = time_now_ms;
}

// returns the current time in ms
//
// so 1 second would be returned as 1000 ms

time_ms w_time::get_time_ms() const
{
	auto time = glfwGetTime();
	return static_cast<time_ms>( time * 1000.0 );
}
