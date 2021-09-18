
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const Vec3 Vec3::zero = Vec3( 0.f, 0.f, 0.f );

Vec3::Vec3()
	: x( 0.f ), y( 0.f ), z( 0.f )
{
}

Vec3::Vec3( int32_t x, int32_t y, int32_t z )
	: x( (float)( x ) ), y( (float)( y ) ), z( (float)( z ) )
{
}

Vec3::Vec3( float_t x, float_t y, float_t z )
	: x( x ), y( y ), z( z )
{
}

Vec3::Vec3( std::string_view str )
{
	Tokenizer tok( str, "," );

	x = Text_Parser::float_from_str( *tok.get_next_token() );
	y = Text_Parser::float_from_str( *tok.get_next_token() );
	z = Text_Parser::float_from_str( *tok.get_next_token() );
}

}