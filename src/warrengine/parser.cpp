
#include "master_pch.h"
#include "master_header.h"

namespace war
{

std::string_view Text_Parser::key_from_str( std::string_view src_string, std::string_view key )
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

int32_t Text_Parser::int_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_int_from_symbol( str );
	}

	// Otherwise, parse the string...
	return String_Util::to_int( str );
}

uint32_t Text_Parser::uint_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_int_from_symbol( str );
	}

	// Otherwise, parse the string...
	return String_Util::to_uint( str.data() );
}

bool Text_Parser::bool_from_str( std::string_view str )
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

float_t Text_Parser::float_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_float_from_symbol( str );
	}

	// Otherwise, parse the string...
	return String_Util::to_float( str );
}

Color Text_Parser::color_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_color_from_symbol( str );
	}

	auto color_str = std::string( str );
	return Color( color_str );
}

Range<float_t> Text_Parser::range_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_range_from_symbol( str );
	}

	// Otherwise, parse the string...

	Tokenizer tok( str, "," );

	Range<float_t> range(
		Text_Parser::float_from_str( *tok.get_next_token() ),
		Text_Parser::float_from_str( *tok.get_next_token() )
	);

	return range;
}

Rect Text_Parser::rect_from_str( std::string_view str )
{
	Tokenizer tok( str, "," );

	Rect rect;
	rect.x = Text_Parser::float_from_str( *tok.get_next_token() );
	rect.y = Text_Parser::float_from_str( *tok.get_next_token() );
	rect.w = Text_Parser::float_from_str( *tok.get_next_token() );
	rect.h = Text_Parser::float_from_str( *tok.get_next_token() );

	return rect;
}

Vec2 Text_Parser::vec2_from_str( std::string_view str )
{
	// If this str is a known symbol, return the value from the lookup table

	if( g_engine->is_symbol_in_map( str ) )
	{
		return g_engine->find_vec2_from_symbol( str );
	}

	// Otherwise, parse the string...

	Tokenizer tok( str, "," );

	Vec2 vec2;
	vec2.x = Text_Parser::float_from_str( *tok.get_next_token() );
	vec2.y = Text_Parser::float_from_str( *tok.get_next_token() );

	return vec2;
}

std::unique_ptr<Timeline_Values> Text_Parser::timeline_from_str( e_timeline_type_t type, std::string_view str )
{
	std::unique_ptr<Timeline_Values> tl = std::make_unique<Timeline_Values>( type );
	tl->clear_key_frames();

	Tokenizer tok( str, "," );

	// Make sure there are an even number of tokens
	assert( ( tok.tokens.size() % 2 ) == 0 );

	while( !tok.is_eos() )
	{
		Timeline_Values_Key_Frame kf;

		kf.pct_marker = Text_Parser::float_from_str( *tok.get_next_token() );

		switch( type )
		{
			case e_timeline_type::float_type:
			{
				kf.float_value = Text_Parser::float_from_str( *tok.get_next_token() );
				break;
			}

			case e_timeline_type::color_type:
			{
				kf.color_value = Text_Parser::color_from_str( *tok.get_next_token() );
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

std::vector<Color> Text_Parser::color_list_from_str( std::string_view str )
{
	std::vector<Color> color_list;

	Tokenizer tok( str, "," );

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
	int32_t repeat_count;
	for( auto& color_value : wk_values )
	{
		repeat_count = 1;

		size_t pos = color_value.find_last_of( ':' );
		if( pos != std::string::npos )
		{
			pos++;
			repeat_count = String_Util::to_int( color_value.substr( pos, color_value.length() - pos ) );
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
