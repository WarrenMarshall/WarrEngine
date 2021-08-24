
namespace war
{

struct string_util final
{
	[[nodiscard]] static std::string_view ltrim( std::string_view str );
	[[nodiscard]] static std::string_view rtrim( std::string_view str );
	[[nodiscard]] static std::string_view trim( std::string_view str );
	[[nodiscard]] static const std::string format_with_commas( float value );
	static void replace_substring( std::string& str, std::string_view old_str, std::string_view new_str );
	static void replace_char( std::string& str, char old_char, char new_char );
	static void erase_char( std::string& str, char char_to_erase );
	[[nodiscard]] static int to_int( std::string_view str );
	[[nodiscard]] static unsigned to_uint( std::string_view str );
	[[nodiscard]] static float to_float( std::string_view str );
};

#define f_commas string_util::format_with_commas

}
