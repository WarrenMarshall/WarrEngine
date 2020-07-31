
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

const std::string w_stringutil::format( const std::string sFormat, ... )
{
    va_list vargs;
    va_start( vargs, sFormat );
    const std::string str = w_stringutil::format_valist( sFormat, vargs );
    va_end( vargs );

    return str;
}

const std::string w_stringutil::format_valist( const std::string sFormat, va_list argp )
{
    const char* const zcFormat = sFormat.c_str();

    // reliably acquire the size from a copy of
    // the variable argument array
    // and a functionally reliable call
    // to mock the formatting

    va_list vaCopy;
    va_copy( vaCopy, argp );
    const size_t iLen = vsnprintf( nullptr, 0, zcFormat, vaCopy );
    va_end( vaCopy );

    // return a formatted string without
    // risking memory mismanagement
    // and without assuming any compiler
    // or platform specific behavior

    std::vector<char> zc( iLen + 1 );
    vsnprintf( zc.data(), zc.size(), zcFormat, argp );

    return std::string( zc.data(), zc.size() - 1 );
}

/*
    utilities for trimming the whitespace off the front and back of a string
*/

const std::string WHITESPACE = " \n\r\t\f\v";

const std::string w_stringutil::ltrim( const std::string& s )
{
    const size_t start = s.find_first_not_of( WHITESPACE );
    return ( start == std::string::npos ) ? "" : s.substr( start );
}

const std::string w_stringutil::rtrim( const std::string& s )
{
    const size_t end = s.find_last_not_of( WHITESPACE );
    return ( end == std::string::npos ) ? "" : s.substr( 0, end + 1 );
}

const std::string w_stringutil::trim( const std::string& s )
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
