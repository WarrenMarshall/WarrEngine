#pragma once

// ----------------------------------------------------------------------------

struct w_parser
{
	static std::string parse_key_value( const std::string& src_string, const std::string& key );
	static bool parse_bool_value( const std::string& str );
	static int parse_int_value( const std::string& str );
	static float parse_float_value( const std::string& str );
	static w_color parse_color_value( const std::string& str );
	static w_range parse_range_value( const std::string& str );
	static w_rect parse_rect_value( const std::string& str );
	static w_vec2 parse_vec2_value( const std::string& str );
	static std::unique_ptr<w_timeline> parse_timeline_value( e_timeline_type type, const std::string& str );
};

// ----------------------------------------------------------------------------
