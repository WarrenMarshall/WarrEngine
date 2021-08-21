
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const color color::white = color( 1.f, 1.f, 1.f );
const color color::black = color( 0.f, 0.f, 0.f );
const color color::dark_red = color( 0.5f, 0.f, 0.f );
const color color::red = color( 1.f, 0.f, 0.f );
const color color::dark_green = color( 0.f, 0.5f, 0.f );
const color color::green = color( 0.f, 1.f, 0.f );
const color color::light_green = color( 0.5f, 1.f, 0.5f );
const color color::dark_blue = color( 0.0f, 0.0f, 0.5f );
const color color::blue = color( 0.f, 0.f, 1.f );
const color color::light_blue = color( 0.5f, 0.5f, 1.f );
const color color::orange = color( 1.f, 0.5f, 0.f );
const color color::yellow = color( 1.f, 1.f, 0.f );
const color color::dark_teal = color( 0.2f, 0.4f, 0.5f );
const color color::teal = color( 0.3f, 0.8f, 1.f );
const color color::dark_grey = color( 0.25f, 0.25f, 0.25f );
const color color::grey = color( 0.5f, 0.5f, 0.5f );
const color color::light_grey = color( 0.75f, 0.75f, 0.75f );
const color color::magenta = color( 0.96f, 0.32f, 0.65f );

color::color( float r, float g, float b, float a )
	: r( r ), g( g ), b( b ), a( a )
{
}

// integer values are assumed to be in the 0-255 range and are converted to float
color::color( int r, int g, int b, int a )
	: color( r* byte_color_to_float, g* byte_color_to_float, b* byte_color_to_float, a* byte_color_to_float )
{
}

// strings can contain any supported kind of data format (hex, int, or floats)
color::color( std::string& str )
{
	assert( !str.empty() );

	if( str[ 0 ] == '@' )
	{
		// strings starting with a '@' char are palette indices
		tokenizer tok( str, "@" );
		int idx = string_util::to_int( std::string( tok.get_next_token().value_or( "0" ) ) );
		*this = make_color( idx );
	}
	else if( str[ 0 ] == '$' )
	{
		// strings starting with a '$' char are hex values
		assert( str.length() == 7 );

		r = string_util::to_uint( "$" + str.substr( 1, 2 ) ) * byte_color_to_float;
		g = string_util::to_uint( "$" + str.substr( 3, 2 ) ) * byte_color_to_float;
		b = string_util::to_uint( "$" + str.substr( 5, 2 ) ) * byte_color_to_float;
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
			tokenizer tok( str, "," );

			r = text_parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			g = text_parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			b = text_parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			a = text_parser::int_from_str( tok.get_next_token().value_or( "1.f" ) ) * byte_color_to_float;
		}
		else
		{
			// remove any 'f' postfixes that may have been added to the
			// color values out of programmer habit

			str.erase( std::remove( str.begin(), str.end(), 'f' ), str.end() );
			tokenizer tok( str, "," );

			r = text_parser::float_from_str( *tok.get_next_token() );
			g = text_parser::float_from_str( *tok.get_next_token() );
			b = text_parser::float_from_str( *tok.get_next_token() );
			a = text_parser::float_from_str( tok.get_next_token().value_or( "1.f" ) );
		}
	}
}

color color::operator*( float v ) const
{
	return color( this->r * v, this->g * v, this->b * v );
}

color color::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

color color::operator-( color v ) const
{
	return color( this->r - v.r, this->g - v.g, this->b - v.b, this->a - v.a );
}

color color::operator-=( color v )
{
	*this = *this - v;
	return *this;
}

color color::operator+( color v ) const
{
	return color( this->r + v.r, this->g + v.g, this->b + v.b, this->a + v.a );
}

color color::operator+=( color v )
{
	*this = *this + v;
	return *this;
}

void color::scale( color& color, float s )
{
	color.r *= s;
	color.g *= s;
	color.b *= s;
}

color color::make( const color& clr, float alpha )
{
	return color( clr.r, clr.g, clr.b, alpha );
}

color color::make( e_pal pal_idx, float alpha )
{
	return make_color( render::palette[ pal_idx ], alpha );
}

}
