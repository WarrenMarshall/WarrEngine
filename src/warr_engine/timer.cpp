
#include "master_pch.h"
#include "master_header.h"

w_timer::w_timer( time_ms interval_ms)
	: interval_ms( interval_ms )
{
	restart();
}

void w_timer::restart()
{
	delta_accum = 0;
	time_last = engine->time->now();
}

// call to see if timer has run over it's duration
//
// this is good for: one-shot timers

bool w_timer::is_elapsed()
{
	return ( engine->time->now() - time_last ) > interval_ms;
}

// call repeatedly to see how many times the timer has elapsed
//
// this is good for: repeating timers

int w_timer::get_elapsed()
{
	delta_accum += ( engine->time->now() - time_last );
	time_last = engine->time->now();
	return (int)( delta_accum / interval_ms );
}
