#pragma once

// ----------------------------------------------------------------------------

struct w_parser
{
	static std::string_view key_from_str( const std::string_view src_string, const std::string_view key );
	static bool bool_from_str( const std::string_view str );
	static int int_from_str( const std::string_view str );
	static float float_from_str( const std::string_view str );
	static w_color color_from_str( const std::string_view str );
	static w_range range_from_str( const std::string_view str );
	static w_rect rect_from_str( const std::string_view str );
	static w_vec2 vec2_from_str( const std::string_view str );
	static w_vec3 vec3_from_str( const std::string_view str );
	static std::unique_ptr<w_timeline> timeline_from_str( e_timeline_type type, const std::string_view str );
};

// ----------------------------------------------------------------------------
