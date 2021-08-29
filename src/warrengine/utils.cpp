
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

const std::string WHITESPACE = " \n\r\t\f\v";

std::string_view String_Util::ltrim( std::string_view s )
{
	const size_t start = s.find_first_not_of( WHITESPACE );
	return ( start == std::string_view::npos ) ? "" : s.substr( start );
}

std::string_view String_Util::rtrim( std::string_view s )
{
	const size_t end = s.find_last_not_of( WHITESPACE );
	return ( end == std::string_view::npos ) ? "" : s.substr( 0, end + 1 );
}

std::string_view String_Util::trim( std::string_view s )
{
	return String_Util::rtrim( String_Util::ltrim( s ) );
}

const std::string String_Util::format_with_commas( float value )
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

void String_Util::replace_substring( std::string& str, std::string_view old_str, std::string_view new_str )
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

void String_Util::replace_char( std::string& str, char old_char, char new_char )
{
	std::replace( str.begin(), str.end(), old_char, new_char );
}

// removes ALL instances of "char_to_erase" from "str"

void String_Util::erase_char( std::string& str, char char_to_erase )
{
	str.erase( std::remove( str.begin(), str.end(), char_to_erase ), str.end() );
}

[[nodiscard]] int String_Util::to_int( std::string_view str )
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

[[nodiscard]] unsigned String_Util::to_uint( std::string_view str )
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

[[nodiscard]] float String_Util::to_float( std::string_view str )
{
	float value = 0.f;

	std::from_chars( str.data(), str.data() + str.size(), value );

	return value;
}


// ----------------------------------------------------------------------------

std::vector<war::Vec2> Geo_Util::generate_convex_shape( int sides, float radius )
{
	std::vector<Vec2> verts;

	float angle = 0;
	float angle_step = 360.f / (float)( sides );

	for( auto x = 0 ; x < sides ; ++x )
	{
		verts.push_back( Vec2::dir_from_angle( angle ) * radius );
		angle += angle_step;
	}

	return verts;
}

// ----------------------------------------------------------------------------
// yoinked from : https://stackoverflow.com/questions/794632/programmatically-get-the-cache-line-size

size_t OS_Util::get_cpu_cache_line_sz()
{
	size_t line_size = 0;
	DWORD buffer_size = 0;
	DWORD i = 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = nullptr;

	GetLogicalProcessorInformation( nullptr, &buffer_size );
	buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc( buffer_size );
	GetLogicalProcessorInformation( &buffer[ 0 ], &buffer_size );

	for( i = 0; i != buffer_size / sizeof( SYSTEM_LOGICAL_PROCESSOR_INFORMATION ); ++i )
	{
		if( buffer[ i ].Relationship == RelationCache and buffer[ i ].Cache.Level == 1 )
		{
			line_size = buffer[ i ].Cache.LineSize;
			break;
		}
	}

	free( buffer );
	return line_size;
}

}