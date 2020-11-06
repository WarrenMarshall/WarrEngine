
// this file exists soley and entirely to generate the precompiled header file

#include "master_pch.h"

float snap_to_pixel( float f )
{
	return std::roundf( f );
}

float str_to_float( const char* str )
{
	return static_cast<float>( strtof( str, ( char** ) nullptr ) );
}

int str_to_int( const char* str )
{
	return static_cast<int>( strtol( str, ( char** ) nullptr, 10 ) );
}

int str_to_int( const std::string& str )
{
	return static_cast<int>( strtol( str.c_str(), ( char** ) nullptr, 10 ) );
}

unsigned str_to_uint( const char* str )
{
	return static_cast<int>( strtoul( str, ( char** ) nullptr, 10 ) );
}

unsigned str_to_uint( const std::string& str )
{
	return static_cast<int>( strtoul( str.c_str(), ( char** ) nullptr, 10 ) );
}

float sin_from_angle( float angle_in_degrees )
{
	return glm::sin( glm::radians( angle_in_degrees ) );
}
