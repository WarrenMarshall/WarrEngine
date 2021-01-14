
#include "master_pch.h"
#include "master_header.h"

w_timer::w_timer( int interval_ms )
	: interval_ms( static_cast<float>( interval_ms ) )
{
	reset();
}

void w_timer::reset()
{
	time_remaining_ms += interval_ms;
}

void w_timer::update()
{
	time_remaining_ms -= FTS::ms_per_step;
}

/*
	call to see if timer has run over it's duration

	this is good for one-shot timers
*/
bool w_timer::is_elapsed()
{
	return ( time_remaining_ms < 0.f );
}

/*
	call repeatedly to see how many times the timer has elapsed since the last call

	this is good for repeating timers
*/
int w_timer::get_elapsed_count()
{
	int elapsed_counter = 0;

	while( time_remaining_ms < 0.f )
	{
		elapsed_counter++;
		time_remaining_ms += interval_ms;
	}

	return elapsed_counter;
}
