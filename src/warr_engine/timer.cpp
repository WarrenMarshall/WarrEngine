
#include "master_pch.h"
#include "master_header.h"

w_timer::w_timer( time_ms interval_ms )
	: interval_ms( interval_ms )
{
	restart();
}

void w_timer::restart()
{
	time_last = engine->time->now();
}

// call to see if timer has run over it's duration
//
// this is good for: one-shot timers

bool w_timer::is_elapsed() const
{
	time_ms delta = ( engine->time->now() - time_last );
	return ( delta >= interval_ms );
}

// call repeatedly to see how many times the timer has elapsed
//
// this is good for: repeating timers

int w_timer::get_elapsed_count()
{
	time_ms delta = ( engine->time->now() - time_last );

	auto num_times_elapsed = (int)( delta / interval_ms );
	time_last += num_times_elapsed * interval_ms;

	return num_times_elapsed;
}
