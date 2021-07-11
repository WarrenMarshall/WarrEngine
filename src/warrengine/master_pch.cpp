
#include "master_pch.h"

namespace war
{

float snap_to_int( float val )
{
	return glm::floor( val + 0.5f );
}

float lerp( float a, float b, float lerp_factor )
{
	float result = ( ( 1.f - lerp_factor ) * a ) + ( lerp_factor * b );
	return result;
}
}
