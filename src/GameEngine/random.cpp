
#include "master_pch.h"
#include "master_header.h"

// inits the random index from the current ticks.
// call this whenever it's convenient to shake up the randomness.

void w_random::seed()
{
    srand( static_cast<unsigned int>( time( nullptr ) ) );
}

// returns an integer value between 0 and RAND_MAX

int w_random::get()
{
    return rand();
}

// returns a float value between 0.0 and 1.0

float w_random::getf()
{
    return get() / static_cast<float>( RAND_MAX );
}

// returns a boolean value

bool w_random::getb()
{
    return static_cast<bool>( (get() % 2) != 0 );
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
