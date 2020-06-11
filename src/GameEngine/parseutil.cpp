
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

std::string w_parser::parse_key_value( const std::string& src_string, const char* key )
{
	size_t idx = src_string.find( key );

	if( idx == std::string::npos )
	{
		log_warning( "%s : no value found for key : [%s]", __FUNCTION__, key );
		return "";
	}

	std::string value = "";
	char* rd_ptr = const_cast<char*>(src_string.c_str()) + idx + strlen( key );

	while( *rd_ptr != ' ' && *rd_ptr != '\n' && *rd_ptr != '\"' )
	{
		value += *rd_ptr;
		rd_ptr++;
	}

	return value;
}

int w_parser::parse_int_value( const std::string& str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_int_from_symbol( str );
	}

	// Otherwise, parse the string into the val
	int val = STRTOL( int, str );

	return val;
}

bool w_parser::parse_bool_value( const std::string& str )
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

float w_parser::parse_float_value( const std::string& str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_float_from_symbol( str );
	}

	// Otherwise, parse the string into the val
	float val = STRTOF( float, str );

	return val;
}
	
w_color w_parser::parse_color_value( const std::string& str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_color_from_symbol( str );
	}

	// Otherwise, parse the string into the val

	w_tokenizer tok( str, '/', "1.0f" );

	w_color color;
	color.r = parse_float_value( tok.get_next_token() );
	color.g = parse_float_value( tok.get_next_token() );
	color.b = parse_float_value( tok.get_next_token() );
	color.a = parse_float_value( tok.get_next_token() );

	return color;
}

w_range w_parser::parse_range_value( const std::string& str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_range_from_symbol( str );
	}

	// Otherwise, parse the string into the val

	w_tokenizer tok( str, ',', "0" );

	w_range range;
	range.min = parse_float_value( tok.get_next_token() );
	range.max = parse_float_value( tok.get_next_token() );

	return range;
}

w_vec2 w_parser::parse_vec2_value( const std::string& str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_vec2_from_symbol( str );
	}

	// Otherwise, parse the string into the val

	w_tokenizer tok( str, ',', "0" );

	w_vec2 vec2;
	vec2.x = parse_float_value( tok.get_next_token() );
	vec2.y = parse_float_value( tok.get_next_token() );

	return vec2;
}

std::unique_ptr<w_timeline> w_parser::parse_timeline_value( e_timeline_type type, const std::string& str )
{
	w_tokenizer tok( str, ',' );

	std::unique_ptr<w_timeline> timeline = std::make_unique<w_timeline>( type );

	int comma_count = static_cast<int>( std::count( str.begin(), str.end(), ',' ) );

	if( ( comma_count % 2 ) == 0 )
	{
		log_error( "%s : '%s' has an odd number of arguments - needs to be even", __FUNCTION__, str.c_str() );
	}

	timeline->kf_clear();
	for( int x = 0 ; x < comma_count ; x += 2 )
	{
		w_keyframe kf;

		kf.pct_marker = w_parser::parse_float_value( tok.get_next_token() );

		switch( type )
		{
			case e_timeline_type::float_type:
			{
				kf.float_value = w_parser::parse_float_value( tok.get_next_token() );
			}
			break;

			case e_timeline_type::color_type:
			{
				kf.color_value = w_parser::parse_color_value( tok.get_next_token() );
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
