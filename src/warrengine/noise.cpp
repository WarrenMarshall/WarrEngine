
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

float noise_perlin::get()
{
	if( seed < 1.0f )
	{
		seed = random::getf();
		idx = random::getf_range( 0.f, 256.f );
	}

	idx += 1.f;
	return glm::perlin( glm::vec2( seed + idx, idx ) );
}

// ----------------------------------------------------------------------------

float noise_simplex::get()
{
	if( seed < 1.0f )
	{
		seed = random::getf();
		idx = random::getf_range( 0.f, 256.f );
	}

	idx += 1.f;
	return glm::simplex( glm::vec2( seed + idx, idx ) );
}

// ----------------------------------------------------------------------------

float noise_random::get()
{
	if( seed < 1.0f )
	{
		seed = random::getf();
		idx = random::getf_range( 0.f, 256.f );
	}

	return random::getf_range( -1.f, 1.f );
}

}
