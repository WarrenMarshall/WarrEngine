
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_accum_value::init()
{
	value = _accumulator = 0.0f;
}

void w_accum_value::update_value( int steps )
{
	value = _accumulator / static_cast<float>( steps );
	_accumulator = 0.0f;
}

void w_accum_value::update_value()
{
	value = _accumulator;
	_accumulator = 0.0f;
}

void w_accum_value::accum( float value )
{
	_accumulator += value;
}

void w_accum_value::inc()
{
	_accumulator += 1.0f;
}
