
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const std::string WHITESPACE = " \n\r\t\f\v";

std::string_view string_util::ltrim( std::string_view s )
{
	const size_t start = s.find_first_not_of( WHITESPACE );
	return ( start == std::string_view::npos ) ? "" : s.substr( start );
}

std::string_view string_util::rtrim( std::string_view s )
{
	const size_t end = s.find_last_not_of( WHITESPACE );
	return ( end == std::string_view::npos ) ? "" : s.substr( 0, end + 1 );
}

std::string_view string_util::trim( std::string_view s )
{
	return string_util::rtrim( string_util::ltrim( s ) );
}

const std::string string_util::format_with_commas( float value )
{
	std::string str = std::format( "{:.0f}", value );
	auto sz = str.size();

	if( sz < 4 )
	{
		// < 1,000
		return str;
	}

	std::string_view str_view = str;

	if( sz < 7 )
	{
		// < 1,000,000
		return std::format( "{},{}", str_view.substr( 0, sz - 3 ), str_view.substr( sz - 3, 3 ) );
	}

	// over a million
	return std::format( "{},{},{}", str_view.substr( 0, sz - 6 ), str_view.substr( sz - 6, 3 ), str_view.substr( sz - 3, 3 ) );
}

// replaces the first occurence of "old_str" with "new_str" in "str".
//
// note : this is for substrings larger than 1 character.
//
//        to replace single characters, use the stl:
//		  std::replace( my_string.begin(), my_string.end(), '\"', ' ' );

void string_util::replace_substring( std::string& str, std::string_view old_str, std::string_view new_str )
{
	// if trying to replace a single character with another single character,
	// call w_string_util::replace_char instead
	assert( old_str.size() > 1 and new_str.size() != 1 );

	auto pos = str.find( old_str );

	if( pos == std::string::npos )
	{
		return;
	}

	std::string wk;

	wk = str.substr( 0, pos );
	wk += new_str;
	wk += str.substr( pos + old_str.size(), str.size() );

	str = wk;
}

void string_util::replace_char( std::string& str, char old_char, char new_char )
{
	std::replace( str.begin(), str.end(), old_char, new_char );
}

// removes ALL instances of "char_to_erase" from "str"

void string_util::erase_char( std::string& str, char char_to_erase )
{
	str.erase( std::remove( str.begin(), str.end(), char_to_erase ), str.end() );
}

[[nodiscard]] int string_util::to_int( std::string_view str )
{
	int value = 0;

	if( str[ 0 ] == '$' )
	{
		std::string_view substr = str.substr( 1, std::string::npos );
		std::from_chars( substr.data(), substr.data() + str.size(), value, 16 );
	}
	else
	{
		std::from_chars( str.data(), str.data() + str.size(), value, 10 );
	}

	return value;
}

[[nodiscard]] unsigned string_util::to_uint( std::string_view str )
{
	unsigned int value = 0;

	if( str[ 0 ] == '$' )
	{
		std::string_view substr = str.substr( 1, std::string::npos );
		std::from_chars( substr.data(), substr.data() + str.size(), value, 16 );
	}
	else
	{
		std::from_chars( str.data(), str.data() + str.size(), value, 10 );
	}

	return value;
}

[[nodiscard]] float string_util::to_float( std::string_view str )
{
	float value = 0.f;

	std::from_chars( str.data(), str.data() + str.size(), value );

	return value;
}

}
