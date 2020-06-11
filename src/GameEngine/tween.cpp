
#include "master_pch.h"
#include "master_header.h"

w_tween::w_tween( e_tween_type type, float start, float end, float step_per_sec )
{
	this->type = type;
	this->start = start;
	this->end = end;
	this->step_per_sec = step_per_sec;
	_dir = 1.0;

	_fval = start;
	_ival = static_cast<int>( round( _fval ) );
}

void w_tween::update_limits()
{
	switch( type )
	{
		case e_tween_type::linear:
		{
			if( _fval >= end )
			{
				_fval = end;
			}

			if( _fval <= start )
			{
				_fval = start;
			}
		}
		break;

		case e_tween_type::loop:
		case e_tween_type::sine:
		case e_tween_type::half_sine:
		{
			if( _fval >= end )
			{
				_fval -= ( end - start );
			}

			if( _fval <= start )
			{
				_fval += ( end - start );
			}
		}
		break;

		case e_tween_type::pingpong:
		{
			if( _fval >= end )
			{
				_dir = -1.0;
				_fval = end - (_fval - end);
			}

			if( _fval <= start )
			{
				_dir = 1.0;
				_fval = start + (start - _fval);
			}
		}
		break;
	}
}

void w_tween::update()
{
	if( use_fts_update )
	{
		return;
	}

	_fval += ( step_per_sec * engine->time->delta_s) * _dir;

	update_limits();

	_ival = static_cast<int>( round( _fval ) );
}

void w_tween::update_fts()
{
	if( !use_fts_update )
	{
		return;
	}

	_fval += ( step_per_sec * (w_time::FTS_step_value_ms / 1000.f)) * _dir;

	update_limits();

	_ival = static_cast<int>( round( _fval ) );
}

void w_tween::reset_to_start()
{
	_fval = start;
	_ival = static_cast<int>( round( _fval ) );
}

int w_tween::get_ival()
{
	return _ival;
}

float w_tween::get_fval()
{
	switch( type )
	{
		case e_tween_type::sine:
			return sin_from_angle( _fval );

		case e_tween_type::half_sine:
			return fabs( sin_from_angle( _fval ) );
	}

	return _fval;
}

/*
	randomizes the current position of this tween somewhere
	between the start and end values.
*/
void w_tween::randomize()
{
	_fval = start + ( ( end - start ) * engine->random->getf() );
	_ival = static_cast<int>( round( _fval ) );
}
