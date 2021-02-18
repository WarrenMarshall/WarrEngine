#pragma once

struct w_string_util
{
	[[nodiscard]] static const std::string_view ltrim( const std::string_view str );
	[[nodiscard]] static const std::string_view rtrim( const std::string_view str );
	[[nodiscard]] static const std::string_view trim( const std::string_view str );
	[[nodiscard]] static const std::string format_with_commas( float value );
	[[nodiscard]] static bool contains_char( std::string_view str, char char_to_find );
};
