
namespace war
{

struct text_parser final
{
	[[nodiscard]] static std::string_view key_from_str( std::string_view src_string, std::string_view key );
	[[nodiscard]] static bool bool_from_str( std::string_view str );
	[[nodiscard]] static int int_from_str( std::string_view str );
	[[nodiscard]] static unsigned uint_from_str( std::string_view str );
	[[nodiscard]] static float float_from_str( std::string_view str );
	[[nodiscard]] static color color_from_str( std::string_view str );
	[[nodiscard]] static range<float> range_from_str( std::string_view str );
	[[nodiscard]] static rect rect_from_str( std::string_view str );
	[[nodiscard]] static vec2 vec2_from_str( std::string_view str );
	[[nodiscard]] static std::unique_ptr<timeline> timeline_from_str( e_timeline_type type, std::string_view str );
	[[nodiscard]] static std::vector<color> color_list_from_str( std::string_view str );
};

}

