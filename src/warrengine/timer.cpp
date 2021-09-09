
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
	time_last = g_engine->clock.now();
}

// call to see if timer has run over it's duration
//
// this is good for: one-shot timers

bool Timer::is_elapsed()
{
	return ( g_engine->clock.now() - time_last ) > interval_ms;
}

// call repeatedly to see how many times the timer has elapsed
//
// this is good for: repeating timers

int32_t Timer::get_elapsed()
{
	delta_accum += ( g_engine->clock.now() - time_last );
	time_last = g_engine->clock.now();
	return (int32_t)( delta_accum / interval_ms );
}

// returns the percentage of completion for this time, expressed as 0-1

float_t Timer::get_pct_complete()
{
	float_t pct = ( g_engine->clock.now() - time_last ) / (float)interval_ms;
	return glm::clamp<float_t>( pct, 0.f, 1.f );
}

}
