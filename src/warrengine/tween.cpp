
#include "master_pch.h"
#include "master_header.h"

namespace war
{

tween::tween( float start, float end, time_ms duration_ms, e_tween_type type, e_tween_via via )
	: start( start ), end( end )
{
	_tween = tweeny::from( start ).to( end ).during( duration_ms );
	set_type( type );
	set_via( via );

	time_last = g_engine->time.now();
}

tween::tween()
{
	time_last = g_engine->time.now();
}

// when a tween is dereferenced, it is updated with the amount of time that has
// passed since the last time. the value is then returned.

float tween::operator*()
{
	time_ms delta = g_engine->time.now() - time_last;
	time_last = g_engine->time.now();

	return _tween.step( (int)( delta ) );
}

void tween::restart()
{
	_tween.seek( 0.f, true );
}

void tween::randomize()
{
	_tween.seek( random::getf(), true );
}

void tween::toggle_direction()
{
	// direction is returned as 1:forward, -1:backward

	if( _tween.direction() == 1 )
	{
		_tween.backward();
	}
	else
	{
		_tween.forward();
	}
}

void tween::set_backwards()
{
	_tween.seek( 1.f, false );
	_tween.backward();
}

// ----------------------------------------------------------------------------
// loop
//
// - when the end is reached, reset to the start

bool on_step_loop( tweeny::tween<float>& tween )
{
	if( tween.progress() >= 1.f )
	{
		tween.seek( 0.f, false );
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
	if( tween.progress() >= 1.f )
	{
		tween.backward();
	}

	if( tween.progress() <= 0.001f )
	{
		tween.forward();
	}

	return false;
}

void tween::set_type( e_tween_type type )
{
	switch( type )
	{
		case tween_type::loop:
		{
			_tween.onStep( on_step_loop );
			break;
		}

		case tween_type::pingpong:
		{
			_tween.onStep( on_step_pingpong );
			break;
		}
	}
}

void tween::set_via( e_tween_via via )
{
	switch( via )
	{
		case tween_via::linear:				_tween.via( tweeny::easing::linear );			break;
		case tween_via::quadratic:			_tween.via( tweeny::easing::quadraticInOut );	break;
		case tween_via::quadratic_in:		_tween.via( tweeny::easing::quadraticIn );		break;
		case tween_via::quadratic_out:		_tween.via( tweeny::easing::quadraticOut );		break;
		case tween_via::cubic:				_tween.via( tweeny::easing::cubicInOut );		break;
		case tween_via::cubic_in:			_tween.via( tweeny::easing::cubicIn );			break;
		case tween_via::cubic_out:			_tween.via( tweeny::easing::cubicOut );			break;
		case tween_via::quartic:			_tween.via( tweeny::easing::quarticInOut );		break;
		case tween_via::quartic_in:			_tween.via( tweeny::easing::quarticIn );		break;
		case tween_via::quartic_out:		_tween.via( tweeny::easing::quarticOut );		break;
		case tween_via::quintic:			_tween.via( tweeny::easing::quinticInOut );		break;
		case tween_via::quintic_in:			_tween.via( tweeny::easing::quinticIn );		break;
		case tween_via::quintic_out:		_tween.via( tweeny::easing::quinticOut );		break;
		case tween_via::sinusoidal:			_tween.via( tweeny::easing::sinusoidalInOut );	break;
		case tween_via::sinusoidal_in:		_tween.via( tweeny::easing::sinusoidalIn );		break;
		case tween_via::sinusoidal_out:		_tween.via( tweeny::easing::sinusoidalOut );	break;
		case tween_via::exponential:		_tween.via( tweeny::easing::exponentialInOut );	break;
		case tween_via::exponential_in:		_tween.via( tweeny::easing::exponentialIn );	break;
		case tween_via::exponential_out:	_tween.via( tweeny::easing::exponentialOut );	break;
		case tween_via::circular:			_tween.via( tweeny::easing::circularInOut );	break;
		case tween_via::circular_in:		_tween.via( tweeny::easing::circularIn );		break;
		case tween_via::circular_out:		_tween.via( tweeny::easing::circularOut );		break;
		case tween_via::bounce:				_tween.via( tweeny::easing::bounceInOut );		break;
		case tween_via::bounce_in:			_tween.via( tweeny::easing::bounceIn );			break;
		case tween_via::bounce_out:			_tween.via( tweeny::easing::bounceOut );		break;
		case tween_via::elastic:			_tween.via( tweeny::easing::elasticInOut );		break;
		case tween_via::elastic_in:			_tween.via( tweeny::easing::elasticIn );		break;
		case tween_via::elastic_out:		_tween.via( tweeny::easing::elasticOut );		break;
		case tween_via::back:				_tween.via( tweeny::easing::backInOut );		break;
		case tween_via::back_in:			_tween.via( tweeny::easing::backIn );			break;
		case tween_via::back_out:			_tween.via( tweeny::easing::backOut );			break;
		default:							assert( false );	// unknown via type
	}
}

}
