
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Random::seed()
{
	std::srand( (uint32_t)std::time( nullptr ) );
}

// returns a float value between 0.0 and 1.0

float Random::getf()
{
	return glm::linearRand( 0.f, 1.f );
}

// returns a boolean value

bool Random::getb()
{
	return ( getf() < 0.5f ) ? true : false;
}

// returns a float value between min and max

float Random::getf_range( float min, float max )
{
	return glm::linearRand( min, max );
}

// returns an integer value between min and max

int32_t Random::geti_range( int32_t min, int32_t max )
{
	return (int32_t)( glm::floor( getf_range( (float)( min ), (float)( max ) ) + 0.5f ) );
}

// returns a vec2 inside of a circle

Vec2 Random::get_random_in_circle( float radius )
{
	auto result = glm::diskRand( radius );
	return { result.x, result.y };
}

// returns a vec2 on the outside of a circle

Vec2 Random::get_random_on_circle( float radius )
{
	auto result = glm::circularRand( radius );
	return { result.x, result.y };
}

Vec2 Random::get_random_unit_vector()
{
	Vec2 v = { Random::getf(), Random::getf() };
	return Vec2::normalize( v );
}

}
