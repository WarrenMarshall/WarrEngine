
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

float_t Perlin_Noise::get()
{
	if( seed < 1.0f )
	{
		seed = Random::getf();
		idx = Random::getf_range( 0.f, 256.f );
	}

	idx += 1.f;
	return glm::perlin( glm::vec2( seed + idx, idx ) );
}

// ----------------------------------------------------------------------------

float_t Simplex_Noise::get()
{
	if( seed < 1.0f )
	{
		seed = Random::getf();
		idx = Random::getf_range( 0.f, 256.f );
	}

	idx += 1.f;
	return glm::simplex( glm::vec2( seed + idx, idx ) );
}

// ----------------------------------------------------------------------------

float_t Random_Noise::get()
{
	if( seed < 1.0f )
	{
		seed = Random::getf();
		idx = Random::getf_range( 0.f, 256.f );
	}

	return Random::getf_range( -1.f, 1.f );
}

}
