
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
