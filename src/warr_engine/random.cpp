
#include "master_pch.h"
#include "master_header.h"

w_random::w_random()
{
    gen = std::mt19937( rd() );
    distr = std::uniform_real_distribution<float>( 0.0f, 1.0f );
}

// returns a float value between 0.0 and 1.0

float w_random::getf()
{
    return distr( gen );
}


// returns a boolean value

bool w_random::getb()
{
    return static_cast<bool>( getf() < 0.5f );
}

// returns a float value between min and max
//
// NOTE : min/max are inclusive

float w_random::getf_range( float min, float max )
{
    assert( min < max );

    return min + ( getf() * ( max - min ) );
}

// returns an integer value between min and max
//
// NOTE : min/max are inclusive

int w_random::geti_range( int min, int max )
{
    assert( min < max );

	return static_cast<int>( round( getf_range( static_cast<float>( min ), static_cast<float>( max ) ) ) );
}
