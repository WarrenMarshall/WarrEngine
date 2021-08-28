
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Timer::Timer( time_ms interval_ms )
	: interval_ms( interval_ms )
{
	restart();
}

void Timer::restart()
{
	delta_accum = 0;
	time_last = g_engine->time.now();
}

// call to see if timer has run over it's duration
//
// this is good for: one-shot timers

bool Timer::is_elapsed()
{
	return ( g_engine->time.now() - time_last ) > interval_ms;
}

// call repeatedly to see how many times the timer has elapsed
//
// this is good for: repeating timers

int Timer::get_elapsed()
{
	delta_accum += ( g_engine->time.now() - time_last );
	time_last = g_engine->time.now();
	return (int)( delta_accum / interval_ms );
}

// returns the percentage of completion for this time, expressed as 0-1

float Timer::get_pct_complete()
{
	float pct = ( g_engine->time.now() - time_last ) / (float)interval_ms;
	return glm::clamp<float>( pct, 0.f, 1.f );
}

}
