
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

/*
    utilities for trimming the whitespace off the front and back of a string
*/

const std::string WHITESPACE = " \n\r\t\f\v";

const std::string_view w_stringutil::ltrim( const std::string_view s )
{
    const size_t start = s.find_first_not_of( WHITESPACE );
    return ( start == std::string_view::npos ) ? "" : s.substr( start );
}

const std::string_view w_stringutil::rtrim( const std::string_view s )
{
    const size_t end = s.find_last_not_of( WHITESPACE );
    return ( end == std::string_view::npos ) ? "" : s.substr( 0, end + 1 );
}

const std::string_view w_stringutil::trim( const std::string_view s )
{
    return w_stringutil::rtrim( w_stringutil::ltrim( s ) );
}

const std::string w_stringutil::format_with_commas( float value )
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
