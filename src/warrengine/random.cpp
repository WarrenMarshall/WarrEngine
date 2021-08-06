
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void random::seed()
{
	std::srand( (unsigned)std::time( nullptr ) );
}

// returns a float value between 0.0 and 1.0

float random::getf()
{
	return glm::linearRand( 0.f, 1.f );
}

// returns a boolean value

bool random::getb()
{
	return ( getf() < 0.5f ) ? true : false;
}

// returns a float value between min and max

float random::getf_range( float min, float max )
{
	return glm::linearRand( min, max );
}

// returns an integer value between min and max

int random::geti_range( int min, int max )
{
	return (int)( glm::floor( getf_range( (float)( min ), (float)( max ) ) + 0.5f ) );
}

// returns a vec2 inside of a circle

vec2 random::get_random_in_circle( float radius )
{
	auto result = glm::diskRand( radius );
	return { result.x, result.y };
}

// returns a vec2 on the outside of a circle

vec2 random::get_random_on_circle( float radius )
{
	auto result = glm::circularRand( radius );
	return { result.x, result.y };
}

vec2 random::get_random_unit_vector()
{
	vec2 v = { random::getf(), random::getf() };
	return vec2::normalize( v );
}

}
