
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

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
	std::string str = fmt::format( "{:.0f}", value );
	auto sz = str.size();

	if( sz < 4 )
	{
		// < 1,000
		return str;
	}
	else if( sz < 7 )
	{
		// < 1,000,000
		return str.substr( 0, sz - 3 ) + "," + str.substr( sz - 3, 3 );
	}
	else
	{
		// over a million
		return str.substr( 0, sz - 6 ) + "," + str.substr( sz - 6, 3 ) + "," + str.substr( sz - 3, 3 );
	}
}
