
#include "master_pch.h"
#include "master_header.h"

namespace war
{

noise::noise()
{
	init();
}

void noise::init()
{
	seed = random::getf();
	idx = random::getf_range( 0.f, 256.f );
}

// ----------------------------------------------------------------------------

float noise_perlin::get()
{
	idx += 1.f;
	return glm::perlin( glm::vec2( seed + idx, idx ) );
}

// ----------------------------------------------------------------------------

float noise_simplex::get()
{
	idx += 1.f;
	return glm::simplex( glm::vec2( seed + idx, idx ) );
}

// ----------------------------------------------------------------------------

float noise_random::get()
{
	return random::getf_range( -1.f, 1.f );
}

}
