
#include "master_pch.h"
#include "master_header.h"

namespace war
{

void Random::seed()
{
	std::srand( (ui32)std::time( nullptr ) );
}

// returns a float value between 0.0 and 1.0

f32 Random::getf()
{
	return glm::linearRand( 0.f, 1.f );
}

// returns a boolean value

bool Random::getb()
{
	return ( getf() < 0.5f ) ? true : false;
}

// returns a float value between min and max

f32 Random::getf_range( f32 min, f32 max )
{
	return glm::linearRand( min, max );
}

// returns an integer value between min and max

i32 Random::geti_range( i32 min, i32 max )
{
	return (i32)( glm::floor( getf_range( (f32)( min ), (f32)( max ) ) + 0.5f ) );
}

// returns a vec2 inside of a circle

Vec2 Random::get_random_in_circle( f32 radius )
{
	auto result = glm::diskRand( radius );
	return { result.x, result.y };
}

// returns a vec2 on the outside of a circle

Vec2 Random::get_random_on_circle( f32 radius )
{
	auto result = glm::circularRand( radius );
	return { result.x, result.y };
}

Vec2 Random::get_random_unit_vector()
{
	Vec2 v = { Random::getf_range( -1.f, 1.f ), Random::getf_range( -1.f, 1.f ) };
	return Vec2::normalize( v );
}

}
