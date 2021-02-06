
#include "master_pch.h"
#include "master_header.h"

w_tween::w_tween( float start, float end, time_ms duration_ms, e_tween_type type, e_tween_via via )
	: start( start ), end( end )
{
	tween = tweeny::from( start ).to( end ).during( duration_ms );
	set_type( type );
	set_via( via );

	time_last = engine->time->now();
}

w_tween::w_tween()
{
	time_last = engine->time->now();
}

// when a tween is dereferenced, it is updated with the amount of time that has
// passed since the last time. the value is then returned.

float w_tween::operator*()
{
	time_ms delta = engine->time->now() - time_last;
	time_last = engine->time->now();

	return glm::clamp( tween.step( static_cast<int>( delta ) ), start, end );
}

void w_tween::restart()
{
	tween.seek( 0.0f, true );
}

void w_tween::randomize()
{
	tween.seek( engine->random->getf(), true );
}

void w_tween::toggle_direction()
{
	// direction is returned as 1:forward, -1:backward

	if( tween.direction() == 1 )
	{
		tween.backward();
	}
	else
	{
		tween.forward();
	}
}

void w_tween::set_backwards()
{
	tween.seek( 1.0f, false );
	tween.backward();
}

// ----------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------
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

void w_tween::set_type( e_tween_type type )
{
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

void w_tween::set_via( e_tween_via via )
{
	switch( via )
	{
		case tween_via::linear:				tween.via( tweeny::easing::linear );			break;
		case tween_via::quadratic:			tween.via( tweeny::easing::quadraticInOut );	break;
		case tween_via::quadratic_in:		tween.via( tweeny::easing::quadraticIn );		break;
		case tween_via::quadratic_out:		tween.via( tweeny::easing::quadraticOut );		break;
		case tween_via::cubic:				tween.via( tweeny::easing::cubicInOut );		break;
		case tween_via::cubic_in:			tween.via( tweeny::easing::cubicIn );			break;
		case tween_via::cubic_out:			tween.via( tweeny::easing::cubicOut );			break;
		case tween_via::quartic:			tween.via( tweeny::easing::quarticInOut );		break;
		case tween_via::quartic_in:			tween.via( tweeny::easing::quarticIn );			break;
		case tween_via::quartic_out:		tween.via( tweeny::easing::quarticOut );		break;
		case tween_via::quintic:			tween.via( tweeny::easing::quinticInOut );		break;
		case tween_via::quintic_in:			tween.via( tweeny::easing::quinticIn );			break;
		case tween_via::quintic_out:		tween.via( tweeny::easing::quinticOut );		break;
		case tween_via::sinusoidal:			tween.via( tweeny::easing::sinusoidalInOut );	break;
		case tween_via::sinusoidal_in:		tween.via( tweeny::easing::sinusoidalIn );		break;
		case tween_via::sinusoidal_out:		tween.via( tweeny::easing::sinusoidalOut );		break;
		case tween_via::exponential:		tween.via( tweeny::easing::exponentialInOut );	break;
		case tween_via::exponential_in:		tween.via( tweeny::easing::exponentialIn );		break;
		case tween_via::exponential_out:	tween.via( tweeny::easing::exponentialOut );	break;
		case tween_via::circular:			tween.via( tweeny::easing::circularInOut );		break;
		case tween_via::circular_in:		tween.via( tweeny::easing::circularIn );		break;
		case tween_via::circular_out:		tween.via( tweeny::easing::circularOut );		break;
		case tween_via::bounce:				tween.via( tweeny::easing::bounceInOut );		break;
		case tween_via::bounce_in:			tween.via( tweeny::easing::bounceIn );			break;
		case tween_via::bounce_out:			tween.via( tweeny::easing::bounceOut );			break;
		case tween_via::elastic:			tween.via( tweeny::easing::elasticInOut );		break;
		case tween_via::elastic_in:			tween.via( tweeny::easing::elasticIn );			break;
		case tween_via::elastic_out:		tween.via( tweeny::easing::elasticOut );		break;
		case tween_via::back:				tween.via( tweeny::easing::backInOut );			break;
		case tween_via::back_in:			tween.via( tweeny::easing::backIn );			break;
		case tween_via::back_out:			tween.via( tweeny::easing::backOut );			break;
		default:							assert( false );	// unknown via type
	}
}
