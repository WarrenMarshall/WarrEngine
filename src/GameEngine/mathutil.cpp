
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_fibonacci::w_fibonacci()
{
	v1 = w_random::geti_range( 1, 9 );
	v2 = v1;

	while( v1 == v2)
		v2 = w_random::geti_range( 1, 9 );

	sum = ( v1 + v2 ) % 10;
}

int w_fibonacci::step()
{
	v1 = v2;
	v2 = sum;

	sum = (v1 + v2) % 10;
	if( sum == 0 )
		sum++;

	return sum;
}

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
