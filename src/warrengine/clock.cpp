
#include "master_pch.h"
#include "master_header.h"

namespace war
{

clock::clock()
{
	init();
}

void clock::init()
{
	time_since_app_start_ms = 0;
	prev_frame_ms = get_ticks();
}

// called at the start of each frame to update the internal time keeping

void clock::update()
{
	// get current time
	current_frame_ms = get_ticks();

	// compute the delta since the last frame
	delta_ms = (float)( current_frame_ms - prev_frame_ms );

	if( g_engine->is_paused() )
	{
		// if the engine has paused, pretend like no time has passed
		delta_ms = 0;
	}
	else
	{
		delta_ms = delta_ms * dilation;
	}

	fts_accum_ms += delta_ms;
	time_since_app_start_ms += delta_ms;

	// save the current time as the previous frame time
	prev_frame_ms = current_frame_ms;
}

// returns the current time in ms
//
// so 1 second would be returned as 1000 ms

time_ms clock::now() const
{
	return (time_ms)time_since_app_start_ms;
}

// returns the current time in ms, up to the very current tick
//
// so 1 second would be returned as 1000 ms

time_ms clock::get_ticks()
{
	auto time = glfwGetTime();
	return (time_ms)( time * 1000.0 );
}

}
