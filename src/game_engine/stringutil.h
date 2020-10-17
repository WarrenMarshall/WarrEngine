#pragma once

struct w_string_util
{
    _NODISCARD static const std::string_view ltrim( const std::string_view str );
    _NODISCARD static const std::string_view rtrim( const std::string_view str );
    _NODISCARD static const std::string_view trim( const std::string_view str );
    _NODISCARD static const std::string format_with_commas( float value );
    _NODISCARD static const std::string replace_char( const std::string& string, char old_char, char new_char );
	_NODISCARD static const std::string remove_char( const std::string& string, char char_to_remove );
};
