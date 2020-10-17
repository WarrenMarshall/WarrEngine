
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

/*
    utilities for trimming the whitespace off the front and back of a string
*/

const std::string WHITESPACE = " \n\r\t\f\v";

const std::string_view w_string_util::ltrim( const std::string_view s )
{
    const size_t start = s.find_first_not_of( WHITESPACE );
    return ( start == std::string_view::npos ) ? "" : s.substr( start );
}

const std::string_view w_string_util::rtrim( const std::string_view s )
{
    const size_t end = s.find_last_not_of( WHITESPACE );
    return ( end == std::string_view::npos ) ? "" : s.substr( 0, end + 1 );
}

const std::string_view w_string_util::trim( const std::string_view s )
{
    return w_string_util::rtrim( w_string_util::ltrim( s ) );
}

const std::string w_string_util::format_with_commas( float value )
{
	std::string numWithCommas = fmt::format( "{:.0f}", value );
	int insertPosition = static_cast<int>( numWithCommas.length() ) - 3;
	while( insertPosition > 0 )
	{
		numWithCommas.insert( insertPosition, "," );
		insertPosition -= 3;
	}

	return numWithCommas;
}

// replaces all occurrences of 'old_char' with 'new_char' in a string, and returns the new string

const std::string w_string_util::replace_char( const std::string& string, char old_char, char new_char )
{
	std::string new_string = string;

	for( auto& ch : new_string )
	{
		if( ch == old_char )
		{
			ch = new_char;
		}
	}

	return new_string;
}

// removes all occurrences of 'char_to_remove' from a string, and returns the new string

const std::string w_string_util::remove_char( const std::string& string, char char_to_remove )
{
	std::string new_string = string;

	new_string.erase( std::remove( new_string.begin(), new_string.end(), char_to_remove ), new_string.end() );

	return new_string;
}
