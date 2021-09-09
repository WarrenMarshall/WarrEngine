namespace war
{

// ----------------------------------------------------------------------------

struct String_Util
{
	[[nodiscard]] static std::string_view ltrim( std::string_view str );
	[[nodiscard]] static std::string_view rtrim( std::string_view str );
	[[nodiscard]] static std::string_view trim( std::string_view str );
	[[nodiscard]] static const std::string format_with_commas( float_t value );
	static void replace_substring( std::string& str, std::string_view old_str, std::string_view new_str );
	static void replace_char( std::string& str, char old_char, char new_char );
	static void erase_char( std::string& str, char char_to_erase );
	[[nodiscard]] static int32_t to_int( std::string_view str );
	[[nodiscard]] static uint32_t to_uint( std::string_view str );
	[[nodiscard]] static float_t to_float( std::string_view str );
};

#define f_commas String_Util::format_with_commas

// ----------------------------------------------------------------------------

struct Geo_Util
{
	[[nodiscard]] static std::vector<Vec2> generate_convex_shape( int32_t sides, float_t radius );
};

// ----------------------------------------------------------------------------

struct OS_Util
{
	[[nodiscard]] static size_t get_cpu_cache_line_sz();
};

}
