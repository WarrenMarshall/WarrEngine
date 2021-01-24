
#include "master_pch.h"
#include "master_header.h"

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

// ----------------------------------------------------------------------------

w_tween::w_tween( e_tween_type type, e_tween_via via, float start, float end, time_ms duration_ms )
	: start( start ), end( end )
{
	tween = tweeny::from( start ).to( end ).during( duration_ms );

	switch( via )
	{
		case tween_via::linear:			tween.via( tweeny::easing::linear );			break;
		case tween_via::quadratic:		tween.via( tweeny::easing::quadraticInOut );	break;
		case tween_via::cubic:			tween.via( tweeny::easing::cubicInOut );		break;
		case tween_via::quartic:		tween.via( tweeny::easing::quarticInOut );		break;
		case tween_via::quintic:		tween.via( tweeny::easing::quinticInOut );		break;
		case tween_via::sinusoidal:		tween.via( tweeny::easing::sinusoidalInOut );	break;
		case tween_via::exponential:	tween.via( tweeny::easing::exponentialInOut );	break;
		case tween_via::circular:		tween.via( tweeny::easing::circularInOut );		break;
		case tween_via::bounce:			tween.via( tweeny::easing::bounceInOut );		break;
		case tween_via::elastic:		tween.via( tweeny::easing::elasticInOut );		break;
		case tween_via::back:			tween.via( tweeny::easing::backInOut );			break;
		default:						assert( false );	// unknown via type
	}

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

	time_last = engine->time->now();
}

w_tween::w_tween()
{
	time_last = engine->time->now();
}

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
