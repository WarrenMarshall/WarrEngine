#pragma once

struct GE_API w_stringutil
{
    static const std::string_view ltrim( const std::string_view str );
    static const std::string_view rtrim( const std::string_view str );
    static const std::string_view trim( const std::string_view str );
    static const std::string format_with_commas( float value );
    static const std::string preprocess( std::string& string );

private:
	w_stringutil() {}
};
