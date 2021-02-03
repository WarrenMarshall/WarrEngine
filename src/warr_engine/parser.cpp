
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
	return str_to_int( str.data() );
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

	auto color_str = std::string( str );
	return w_color( color_str );
}

w_range w_parser::range_from_str( const std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( engine->is_symbol_in_map( str ) )
	{
		return engine->find_range_from_symbol( str );
	}

	// Otherwise, parse the string...

	w_tokenizer tok( str, ',' );

	w_range range;
	range.start = w_parser::float_from_str( *tok.get_next_token() );
	range.end = w_parser::float_from_str( *tok.get_next_token() );

	return range;
}

w_rect w_parser::rect_from_str( const std::string_view str )
{
	w_tokenizer tok( str, ',' );

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

	w_tokenizer tok( str, ',' );

	w_vec2 vec2;
	vec2.x = w_parser::float_from_str( *tok.get_next_token() );
	vec2.y = w_parser::float_from_str( *tok.get_next_token() );

	return vec2;
}

std::unique_ptr<w_timeline> w_parser::timeline_from_str( e_timeline_type type, const std::string_view str )
{
	std::unique_ptr<w_timeline> timeline = std::make_unique<w_timeline>( type );
	timeline->kf_clear();

	w_tokenizer tok( str, ',' );

	// Make sure there are an even number of tokens
	assert( (tok.tokens.size() % 2) == 0 );

	while( !tok.is_eos() )
	{
		w_keyframe kf;

		kf.pct_marker = w_parser::float_from_str( *tok.get_next_token() );

		switch( type )
		{
			case timeline_type::float_type:
			{
				kf.value = w_parser::float_from_str( *tok.get_next_token() );
			}
			break;

			case timeline_type::color_type:
			{
				kf.value = w_parser::color_from_str( *tok.get_next_token() );
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

std::vector<w_color> w_parser::color_list_from_str( const std::string_view str )
{
	std::vector<w_color> color_list;

	w_tokenizer tok( str, ',' );

	std::vector<std::string> wk_values;
	while( true )
	{
		auto val = tok.get_next_token();

		if( !val.has_value() )
		{
			break;
		}

		char ch = ( *val )[ 0 ];
		if( ( ch >= '0' && ch <= '9' ) || ch == '.' )
		{
			std::string composited_color = std::string( *val );
			composited_color += ",";
			composited_color += *tok.get_next_token();
			composited_color += ",";
			composited_color += *tok.get_next_token();

			wk_values.emplace_back( composited_color );
		}
		else
		{
			wk_values.emplace_back( std::string( *val ) );
		}
	}

	// repeating entries

	std::vector<std::string> color_values;
	int repeat_count;
	for( auto& color_value : wk_values )
	{
		repeat_count = 1;

		size_t pos = color_value.find_last_of( ':' );
		if( pos != std::string::npos )
		{
			pos++;
			repeat_count = str_to_int( color_value.substr( pos, color_value.length() - pos ) );
			color_value = color_value.substr( 0, pos - 1 );
		}

		while( repeat_count > 0 )
		{
			color_values.emplace_back( color_value );
			repeat_count--;
		}
	}

	// replace symbols with real values

	for( const auto& iter : color_values )
	{
		auto val = iter;

		if( engine->is_symbol_in_map( val ) )
		{
			val = *engine->find_string_from_symbol( val );
		}

		color_list.emplace_back( w_color( val ) );
	}

	return color_list;
}

// replaces the first occurence of "old" with "new" and returns the new string.
//
// returns TRUE if a replace was done, FALSE if nothing happened

bool w_parser::replace_substring( std::string& str, const std::string_view old_str, const std::string_view new_str )
{
	auto pos = str.find( old_str );

	if( pos == std::string::npos )
	{
		return false;
	}

	std::string wk;

	wk = str.substr( 0, pos );
	wk += new_str;
	wk += str.substr( pos + old_str.size(), str.size() );

	str = wk;

	return true;
}
