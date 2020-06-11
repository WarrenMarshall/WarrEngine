#pragma once

struct w_stringutil
{
    static const std::string format( const std::string sFormat, ... );
    static const std::string format_valist( const std::string sFormat, va_list argp );
    static const std::string ltrim( const std::string& str );
    static const std::string rtrim( const std::string& str );
    static const std::string trim( const std::string& str );

    template<class T>
    static std::string format_with_commas( T value, const std::string& format )
    {
        std::string numWithCommas = w_stringutil::format( format, value );
        int insertPosition = static_cast<int>( numWithCommas.length() ) - 3;
        while( insertPosition > 0 )
        {
            numWithCommas.insert( insertPosition, "," );
            insertPosition -= 3;
        }

        return numWithCommas;
    }

private:
	w_stringutil() {}
};
