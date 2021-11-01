
#include "master_pch.h"

namespace war
{

Timer::Timer( time_ms interval_ms, bool start_elapsed )
	: interval_ms( interval_ms )
{
	restart();

	if( start_elapsed )
	{
		time_last = g_engine->clock.now() + interval_ms;
	}
}

void Timer::restart()
{
	delta_accum = 0;
	time_last = g_engine->clock.now();
}

// call to see if timer has run over it's duration
//
// best for: one-shot timers

bool Timer::is_elapsed()
{
	return ( g_engine->clock.now() - time_last ) > interval_ms;
}

// call repeatedly to see how many times the timer has elapsed
//
// best for: repeating timers

i32 Timer::get_elapsed()
{
	delta_accum += ( g_engine->clock.now() - time_last );
	time_last = g_engine->clock.now();
	return (i32)( delta_accum / interval_ms );
}

// returns the percentage of completion for this time, expressed as 0-1

f32 Timer::get_pct_complete()
{
	f32 pct = ( g_engine->clock.now() - time_last ) / (f32)interval_ms;
	return glm::clamp<f32>( pct, 0.f, 1.f );
}

}
