
#include "master_pch.h"

namespace war
{

// ----------------------------------------------------------------------------

f32 Perlin_Noise::get()
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

f32 Simplex_Noise::get()
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

f32 Random_Noise::get()
{
	if( seed < 1.0f )
	{
		seed = Random::getf();
		idx = Random::getf_range( 0.f, 256.f );
	}

	return Random::getf_range( -1.f, 1.f );
}

}
