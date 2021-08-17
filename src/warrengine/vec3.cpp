
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const vec3 vec3::zero = vec3( 0.f, 0.f, 0.f );

vec3::vec3()
	: x( 0.f ), y( 0.f ), z( 0.f )
{
}

vec3::vec3( int x, int y, int z )
	: x( (float)( x ) ), y( (float)( y ) ), z( (float)( z ) )
{
}

vec3::vec3( float x, float y, float z )
	: x( x ), y( y ), z( z )
{
}

vec3::vec3( std::string_view str )
{
	tokenizer tok( str, "," );

	x = text_parser::float_from_str( *tok.get_next_token() );
	y = text_parser::float_from_str( *tok.get_next_token() );
	z = text_parser::float_from_str( *tok.get_next_token() );
}

}
