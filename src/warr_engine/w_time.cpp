
#include "master_pch.h"
#include "master_header.h"

w_time::w_time()
{
	init();
}

void w_time::init()
{
	prev_frame_ms = get_ticks();
}

/*
	called at the start of each frame to update the internal time keeping
*/
void w_time::update()
{
	uint64_t time_now_ms = get_ticks();

	delta_ms = static_cast<float>( time_now_ms - prev_frame_ms );
	delta_ms *= dilation;

	prev_frame_ms = time_now_ms;

	if( engine->is_paused )
	{
		delta_ms = 0.0f;
	}

	fts_accum_ms += delta_ms;
}

// returns the current time in ms
//
// so 1 second would be returned as 1000 ticks

uint64_t w_time::get_ticks()
{
	return static_cast<uint64_t>( glfwGetTime() * 1000.0f );
}
