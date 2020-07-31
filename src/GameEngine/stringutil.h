#pragma once

struct w_stringutil
{
    static const std::string format( const std::string sFormat, ... );
    static const std::string format_valist( const std::string sFormat, va_list argp );
    static const std::string ltrim( const std::string& str );
    static const std::string rtrim( const std::string& str );
    static const std::string trim( const std::string& str );
    static const std::string format_with_commas( float value );

private:
	w_stringutil() {}
};
