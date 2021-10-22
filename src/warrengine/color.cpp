
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const Color Color::white = Color( 1.f, 1.f, 1.f );
const Color Color::black = Color( 0.f, 0.f, 0.f );
const Color Color::dark_red = Color( 0.5f, 0.f, 0.f );
const Color Color::red = Color( 1.f, 0.f, 0.f );
const Color Color::dark_green = Color( 0.f, 0.5f, 0.f );
const Color Color::green = Color( 0.f, 1.f, 0.f );
const Color Color::light_green = Color( 0.5f, 1.f, 0.5f );
const Color Color::dark_blue = Color( 0.0f, 0.0f, 0.5f );
const Color Color::blue = Color( 0.f, 0.f, 1.f );
const Color Color::light_blue = Color( 0.5f, 0.5f, 1.f );
const Color Color::orange = Color( 1.f, 0.5f, 0.f );
const Color Color::yellow = Color( 1.f, 1.f, 0.f );
const Color Color::dark_teal = Color( 0.2f, 0.4f, 0.5f );
const Color Color::teal = Color( 0.3f, 0.8f, 1.f );
const Color Color::dark_grey = Color( 0.25f, 0.25f, 0.25f );
const Color Color::grey = Color( 0.5f, 0.5f, 0.5f );
const Color Color::light_grey = Color( 0.75f, 0.75f, 0.75f );
const Color Color::magenta = Color( 0.96f, 0.32f, 0.65f );

Color::Color( f32 r, f32 g, f32 b, f32 a )
	: r( r ), g( g ), b( b ), a( a )
{
}

Color::Color( f32 v, f32 a )
	: r( v ), g( v ), b( v ), a( a )
{
}

// integer values are assumed to be in the 0-255 range and are converted to float
Color::Color( i32 r, i32 g, i32 b, i32 a )
	: Color( byte_color_to_float( r ), byte_color_to_float( g ), byte_color_to_float( b ), byte_color_to_float( a ) )
{
}

// strings can contain any supported kind of data format (hex, int, or floats)
Color::Color( std::string& str )
{
	assert( !str.empty() );

	if( str[ 0 ] == '@' )
	{
		// strings starting with a '@' char are palette indices
		Tokenizer tok( str, "@" );
		i32 idx = String_Util::to_int( std::string( tok.get_next_token().value_or( "0" ) ) );
		*this = make_color( (e_pal)idx );
	}
	else if( str[ 0 ] == '$' )
	{
		// strings starting with a '$' char are hex values
		assert( str.length() == 7 );

		r = byte_color_to_float( String_Util::to_uint( "$" + str.substr( 1, 2 ) ) );
		g = byte_color_to_float( String_Util::to_uint( "$" + str.substr( 3, 2 ) ) );
		b = byte_color_to_float( String_Util::to_uint( "$" + str.substr( 5, 2 ) ) );
	}
	else
	{
		// in case this is a set of values, remove the surrounding braces.
		str.erase( std::remove( str.begin(), str.end(), '[' ), str.end() );
		str.erase( std::remove( str.begin(), str.end(), ']' ), str.end() );

		// if string contains a "%" marker anywhere, we assume this to
		// be a set of integer values. Otherwise, they are treated as floats.
		//
		// note : you can't mix-and-match

		if( str.find_first_of( '%' ) != std::string::npos )
		{
			str.erase( std::remove( str.begin(), str.end(), '%' ), str.end() );
			Tokenizer tok( str, "," );

			r = byte_color_to_float( Text_Parser::int_from_str( *tok.get_next_token() ) );
			g = byte_color_to_float( Text_Parser::int_from_str( *tok.get_next_token() ) );
			b = byte_color_to_float( Text_Parser::int_from_str( *tok.get_next_token() ) );
			a = byte_color_to_float( Text_Parser::int_from_str( tok.get_next_token().value_or( "1.f" ) ) );
		}
		else
		{
			// remove any 'f' postfixes that may have been added to the
			// color values out of programmer habit

			str.erase( std::remove( str.begin(), str.end(), 'f' ), str.end() );
			Tokenizer tok( str, "," );

			r = Text_Parser::float_from_str( *tok.get_next_token() );
			g = Text_Parser::float_from_str( *tok.get_next_token() );
			b = Text_Parser::float_from_str( *tok.get_next_token() );
			a = Text_Parser::float_from_str( tok.get_next_token().value_or( "1.f" ) );
		}
	}
}

Color Color::operator*( f32 v ) const
{
	return Color( this->r * v, this->g * v, this->b * v );
}

Color Color::operator*=( f32 v )
{
	*this = *this * v;
	return *this;
}

Color Color::operator-( Color v ) const
{
	return Color( this->r - v.r, this->g - v.g, this->b - v.b, this->a - v.a );
}

Color Color::operator-=( Color v )
{
	*this = *this - v;
	return *this;
}

Color Color::operator+( Color v ) const
{
	return Color( this->r + v.r, this->g + v.g, this->b + v.b, this->a + v.a );
}

Color Color::operator+=( Color v )
{
	*this = *this + v;
	return *this;
}

void Color::scale( Color& color, f32 s )
{
	color.r *= s;
	color.g *= s;
	color.b *= s;
}

Color Color::make( const Color& clr, f32 alpha )
{
	return Color( clr.r, clr.g, clr.b, alpha );
}

Color Color::make( e_pal pal_idx, f32 alpha )
{
	return make_color( Render::palette[ (i32)pal_idx ], alpha );
}

}
