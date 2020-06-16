
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_fibonacci::w_fibonacci()
{
	v1 = engine->random->geti_range( 1, 9 );
	v2 = v1;

	while( v1 == v2)
		v2 = engine->random->geti_range( 1, 9 );

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
	value = accumulator = 0.0f;
}

void w_accum_value::update_value( int steps )
{
	value = accumulator / static_cast<float>( steps );
	accumulator = 0.0f;
}

void w_accum_value::update_value()
{
	value = accumulator;
	accumulator = 0.0f;
}

void w_accum_value::accum( float value )
{
	accumulator += value;
}

void w_accum_value::inc()
{
	accumulator += 1.0f;
}
