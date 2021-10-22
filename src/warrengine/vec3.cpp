
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const Vec3 Vec3::zero = Vec3( 0.f, 0.f, 0.f );

Vec3::Vec3()
	: x( 0.f ), y( 0.f ), z( 0.f )
{
}

Vec3::Vec3( i32 x, i32 y, i32 z )
	: x( (f32)( x ) ), y( (f32)( y ) ), z( (f32)( z ) )
{
}

Vec3::Vec3( f32 x, f32 y, f32 z )
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
