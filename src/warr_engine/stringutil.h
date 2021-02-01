#pragma once

struct w_string_util
{
	[[nodiscard]] static const std::string_view ltrim( const std::string_view str );
	[[nodiscard]] static const std::string_view rtrim( const std::string_view str );
	[[nodiscard]] static const std::string_view trim( const std::string_view str );
	[[nodiscard]] static const std::string format_with_commas( float value );
	[[nodiscard]] static const std::string replace_char( const std::string& string, char old_char, char new_char );
	[[nodiscard]] static const std::string remove_char( const std::string& string, char char_to_remove );
	[[nodiscard]] static const bool contains_char( const std::string& string, char char_to_find );
};
