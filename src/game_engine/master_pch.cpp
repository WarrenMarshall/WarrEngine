
// this file exists soley and entirely to generate the precompiled header file

#include "master_pch.h"

float snap_to_pixel( float f )
{
	return std::roundf( f );
}

int str_to_int( const char* str )
{
	return static_cast<int>( strtol( str, ( char** ) nullptr, 10 ) );
}
