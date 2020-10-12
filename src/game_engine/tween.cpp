
#include "master_pch.h"
#include "master_header.h"

// loop
//
// - when the end is reached, reset to the start

bool on_step_loop( tweeny::tween<float>& tween )
{
	if( tween.progress() >= 1.0f )
	{
		tween.seek( 0.0f, false );
	}

	return false;
}

// pingpong
//
// - when end is reached, start moving backwards
// - when start is reached, start moving forwards

bool on_step_pingpong( tweeny::tween<float>& tween )
{
	if( tween.progress() >= 1.0f )
	{
		tween.backward();
	}

	if( tween.progress() <= 0.001f )
	{
		tween.forward();
	}

	return false;
}

w_tween::w_tween( e_tween_type type, float start, float end, int duration_ms )
{
	tween = tweeny::from( start ).to( end ).during( duration_ms );

	switch( type )
	{
		case tween_type::loop:
		{
			tween.onStep( on_step_loop );
		}
		break;

		case tween_type::pingpong:
		{
			tween.onStep( on_step_pingpong );
		}
		break;
	}
}

void w_tween::update()
{
	current_val = tween.step( static_cast<int>( w_time::FTS_step_value_ms ) );
}

float w_tween::get_fval()
{
	return current_val;
}

int w_tween::get_ival()
{
	return static_cast<int>( get_fval() );
}

int w_tween::get_ival( int low, int high )
{
	// tweeny can sometimes go beyond the limits of the range you've given it,
	// especially if you're using certain easings. if that will cause trouble,
	// use this clamped version of get_ival instead.
	return std::clamp<int>( static_cast<int>( get_fval() ), low, high );
}

bool w_tween::is_negative()
{
	return std::signbit( get_fval() );
}

void w_tween::reset_to_start()
{
	tween.seek( 0.0f, true );
}

void w_tween::randomize()
{

	tween.seek( w_random::getf(), true );
}
