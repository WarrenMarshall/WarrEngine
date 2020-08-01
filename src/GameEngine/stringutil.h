#pragma once

struct w_stringutil
{
    static const std::string_view ltrim( const std::string_view str );
    static const std::string_view rtrim( const std::string_view str );
    static const std::string_view trim( const std::string_view str );
    static const std::string format_with_commas( float value );

private:
	w_stringutil() {}
};
