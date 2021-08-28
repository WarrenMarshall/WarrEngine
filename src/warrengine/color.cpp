
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

Color::Color( float r, float g, float b, float a )
	: r( r ), g( g ), b( b ), a( a )
{
}

// integer values are assumed to be in the 0-255 range and are converted to float
Color::Color( int r, int g, int b, int a )
	: Color( r* byte_color_to_float, g* byte_color_to_float, b* byte_color_to_float, a* byte_color_to_float )
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
		int idx = String_Util::to_int( std::string( tok.get_next_token().value_or( "0" ) ) );
		*this = make_color( idx );
	}
	else if( str[ 0 ] == '$' )
	{
		// strings starting with a '$' char are hex values
		assert( str.length() == 7 );

		r = String_Util::to_uint( "$" + str.substr( 1, 2 ) ) * byte_color_to_float;
		g = String_Util::to_uint( "$" + str.substr( 3, 2 ) ) * byte_color_to_float;
		b = String_Util::to_uint( "$" + str.substr( 5, 2 ) ) * byte_color_to_float;
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

			r = Text_Parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			g = Text_Parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			b = Text_Parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			a = Text_Parser::int_from_str( tok.get_next_token().value_or( "1.f" ) ) * byte_color_to_float;
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

Color Color::operator*( float v ) const
{
	return Color( this->r * v, this->g * v, this->b * v );
}

Color Color::operator*=( float v )
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

void Color::scale( Color& color, float s )
{
	color.r *= s;
	color.g *= s;
	color.b *= s;
}

Color Color::make( const Color& clr, float alpha )
{
	return Color( clr.r, clr.g, clr.b, alpha );
}

Color Color::make( e_pal_t pal_idx, float alpha )
{
	return make_color( Render::palette[ pal_idx ], alpha );
}

}
