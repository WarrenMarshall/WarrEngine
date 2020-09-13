
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

std::string_view w_parser::key_from_str( std::string_view src_string, std::string_view key )
{
	size_t idx = src_string.find( key.data() );

	if( idx == std::string_view::npos )
	{
		log_warning( "No value found for key : [{}]", key );
		return "";
	}

	auto end_idx = src_string.find_first_of( " \n\"", idx + key.length() );
	std::string_view value = src_string.substr( idx + key.length(), end_idx - ( idx + key.length() ) );

	return value;
}

int w_parser::int_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_int_from_symbol( str );
	}

	// Otherwise, parse the string...
	return static_cast<int>( strtol( str.data(), ( char** ) nullptr, 10 ) );
}

bool w_parser::bool_from_str( const std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_int_from_symbol( str ) > 0;
	}

	if( str == "true" )
	{
		return true;
	}

	return false;
}

float w_parser::float_from_str( const std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_float_from_symbol( str );
	}

	// Otherwise, parse the string...
	return static_cast<float>( strtof( str.data(), ( char** ) nullptr ) );
}

w_color w_parser::color_from_str( const std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_color_from_symbol( str );
	}

	// colors in string can either be delimited by slashes or commas.
	//
	// if we find slashes, assume slashes. otherwise, commas.

	size_t slashes = std::count( str.begin(), str.end(), '/' );

	char delimiter = ',';
	if( slashes )
	{
		delimiter = '/';
	}

	w_tokenizer tok( str, delimiter );

	w_color color;
	color.r = w_parser::float_from_str( *tok.get_next_token() );
	color.g = w_parser::float_from_str( *tok.get_next_token() );
	color.b = w_parser::float_from_str( *tok.get_next_token() );
	color.a = w_parser::float_from_str( tok.get_next_token().value_or( "1.0f") );

	return color;
}

w_range w_parser::range_from_str( const std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_range_from_symbol( str );
	}

	// Otherwise, parse the string...

	w_tokenizer tok( str, ',', "0" );

	w_range range;
	range.min = w_parser::float_from_str( *tok.get_next_token() );
	range.max = w_parser::float_from_str( *tok.get_next_token() );

	return range;
}

w_rect w_parser::rect_from_str( const std::string_view str )
{
	w_tokenizer tok( str, ',', "0" );

	w_rect rect;
	rect.x = w_parser::float_from_str( *tok.get_next_token() );
	rect.y = w_parser::float_from_str( *tok.get_next_token() );
	rect.w = w_parser::float_from_str( *tok.get_next_token() );
	rect.h = w_parser::float_from_str( *tok.get_next_token() );

	return rect;
}

w_vec2 w_parser::vec2_from_str( const std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_vec2_from_symbol( str );
	}

	// Otherwise, parse the string...

	w_tokenizer tok( str, ',', "0" );

	w_vec2 vec2;
	vec2.x = w_parser::float_from_str( *tok.get_next_token() );
	vec2.y = w_parser::float_from_str( *tok.get_next_token() );

	return vec2;
}

w_vec3 w_parser::vec3_from_str( const std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_vec3_from_symbol( str );
	}

	// Otherwise, parse the string...

	w_tokenizer tok( str, ',', "0" );

	w_vec3 vec3;
	vec3.x = w_parser::float_from_str( *tok.get_next_token() );
	vec3.y = w_parser::float_from_str( *tok.get_next_token() );
	vec3.z = w_parser::float_from_str( *tok.get_next_token() );

	return vec3;
}

std::unique_ptr<w_timeline> w_parser::timeline_from_str( e_timeline_type type, const std::string_view str )
{
	int comma_count = static_cast<int>( std::count( str.begin(), str.end(), ',' ) );
	if( ( comma_count % 2 ) == 0 )
		log_error( "'{}' has an odd number of arguments - needs to be even", str );

	std::unique_ptr<w_timeline> timeline = std::make_unique<w_timeline>( type );
	timeline->kf_clear();

	w_tokenizer tok( str, ',' );

	for( int x = 0 ; x < comma_count ; x += 2 )
	{
		w_keyframe kf;

		kf.pct_marker = w_parser::float_from_str( *tok.get_next_token() );

		switch( type )
		{
			case timeline_type::float_type:
			{
				kf.float_value = w_parser::float_from_str( *tok.get_next_token() );
			}
			break;

			case timeline_type::color_type:
			{
				kf.color_value = w_parser::color_from_str( *tok.get_next_token() );
			}
			break;

			default:
			{
				assert( false );	// this type needs to be handled
			}
			break;
		}

		timeline->kf_add( kf );
	}

	return timeline;
}
