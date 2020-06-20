
#include "master_pch.h"
#include "master_header.h"

w_timer::w_timer( int interval_ms )
	: interval_ms( static_cast<float>( interval_ms ) )
{
	reset();
}

void w_timer::reset()
{
	time_remaining_ms = interval_ms;
}

void w_timer::update_fts()
{
	if( !use_fts_update )
	{
		return;
	}

	time_remaining_ms -= w_time::FTS_step_value_ms;
}

void w_timer::update()
{
	if( use_fts_update )
	{
		return;
	}

	time_remaining_ms -= engine->time->FTS_step_value_ms;
}

/*
	call to see if timer has run over it's duration

	this is good for one-shot timers
*/
bool w_timer::is_elapsed()
{
	if( time_remaining_ms < 0.f )
	{
		return true;
	}

	return false;
}
	
/*
	call repeatedly to see how many times the timer has elapsed since the last call

	this is good for repeating timers
*/
int w_timer::get_elapsed_count()
{
	int count = 0;

	while( time_remaining_ms < 0.f )
	{
		count++;
		time_remaining_ms += interval_ms;
	}

	return count;
}
