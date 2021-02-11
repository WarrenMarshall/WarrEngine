#pragma once

struct w_string_util
{
	_NODISCARD static const std::string_view ltrim( const std::string_view str );
	_NODISCARD static const std::string_view rtrim( const std::string_view str );
	_NODISCARD static const std::string_view trim( const std::string_view str );
	_NODISCARD static const std::string format_with_commas( float value );
	_NODISCARD static bool contains_char( std::string_view str, char char_to_find );
};
