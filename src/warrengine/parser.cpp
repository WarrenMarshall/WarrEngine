
#include "master_pch.h"
#include "master_header.h"

namespace war
{

std::string_view text_parser::key_from_str( std::string_view src_string, std::string_view key )
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

int text_parser::int_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_int_from_symbol( str );
	}

	// Otherwise, parse the string...
	return string_util::to_int( str );
}

unsigned text_parser::uint_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_int_from_symbol( str );
	}

	// Otherwise, parse the string...
	return string_util::to_uint( str.data() );
}

bool text_parser::bool_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_int_from_symbol( str ) > 0;
	}

	if( str == "true" )
	{
		return true;
	}

	return false;
}

float text_parser::float_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_float_from_symbol( str );
	}

	// Otherwise, parse the string...
	return string_util::to_float( str );
}

color text_parser::color_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_color_from_symbol( str );
	}

	auto color_str = std::string( str );
	return color( color_str );
}

range<float> text_parser::range_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_range_from_symbol( str );
	}

	// Otherwise, parse the string...

	tokenizer tok( str, "," );

	range<float> range(
		text_parser::float_from_str( *tok.get_next_token() ),
		text_parser::float_from_str( *tok.get_next_token() )
	);

	return range;
}

rect text_parser::rect_from_str( std::string_view str )
{
	tokenizer tok( str, "," );

	rect rect;
	rect.x = text_parser::float_from_str( *tok.get_next_token() );
	rect.y = text_parser::float_from_str( *tok.get_next_token() );
	rect.w = text_parser::float_from_str( *tok.get_next_token() );
	rect.h = text_parser::float_from_str( *tok.get_next_token() );

	return rect;
}

vec2 text_parser::vec2_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_vec2_from_symbol( str );
	}

	// Otherwise, parse the string...

	tokenizer tok( str, "," );

	vec2 vec2;
	vec2.x = text_parser::float_from_str( *tok.get_next_token() );
	vec2.y = text_parser::float_from_str( *tok.get_next_token() );

	return vec2;
}

std::unique_ptr<timeline> text_parser::timeline_from_str( e_timeline_type type, std::string_view str )
{
	std::unique_ptr<timeline> tl = std::make_unique<timeline>( type );
	tl->clear_key_frames();

	tokenizer tok( str, "," );

	// Make sure there are an even number of tokens
	assert( ( tok.tokens.size() % 2 ) == 0 );

	while( !tok.is_eos() )
	{
		timeline_key_frame kf;

		kf.pct_marker = text_parser::float_from_str( *tok.get_next_token() );

		switch( type )
		{
			case timeline_type::float_type:
			{
				kf.float_value = text_parser::float_from_str( *tok.get_next_token() );
				break;
			}

			case timeline_type::color_type:
			{
				kf.color_value = text_parser::color_from_str( *tok.get_next_token() );
				break;
			}

			default:
			{
				assert( false );	// this type needs to be handled
				break;
			}
		}

		tl->add_key_frame( kf );
	}

	return tl;
}

std::vector<color> text_parser::color_list_from_str( std::string_view str )
{
	std::vector<color> color_list;

	tokenizer tok( str, "," );

	std::vector<std::string> wk_values;
	while( true )
	{
		auto val = tok.get_next_token();

		if( !val.has_value() )
		{
			break;
		}

		char ch = ( *val )[ 0 ];
		if( ( ch >= '0' and ch <= '9' ) or ch == '.' )
		{
			std::string composited_color = std::string( *val );
			composited_color += ",";
			composited_color += *tok.get_next_token();
			composited_color += ",";
			composited_color += *tok.get_next_token();

			wk_values.push_back( composited_color );
		}
		else
		{
			wk_values.emplace_back( *val );
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
			repeat_count = string_util::to_int( color_value.substr( pos, color_value.length() - pos ) );
			color_value = color_value.substr( 0, pos - 1 );
		}

		while( repeat_count > 0 )
		{
			color_values.push_back( color_value );
			repeat_count--;
		}
	}

	// replace symbols with real values

	for( auto& iter : color_values )
	{
		if( g_engine->is_symbol_in_map( iter ) )
		{
			iter = *g_engine->find_string_from_symbol( iter );
		}

		color_list.emplace_back( iter );
	}

	return color_list;
}

}
