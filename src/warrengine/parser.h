
namespace war
{

struct Text_Parser
{
	[[nodiscard]] static std::string_view key_from_str( std::string_view src_string, std::string_view key );
	[[nodiscard]] static bool bool_from_str( std::string_view str );
	[[nodiscard]] static int32_t int_from_str( std::string_view str );
	[[nodiscard]] static uint32_t uint_from_str( std::string_view str );
	[[nodiscard]] static float_t float_from_str( std::string_view str );
	[[nodiscard]] static Color color_from_str( std::string_view str );
	[[nodiscard]] static Range<float_t> range_from_str( std::string_view str );
	[[nodiscard]] static Rect rect_from_str( std::string_view str );
	[[nodiscard]] static Vec2 vec2_from_str( std::string_view str );
	[[nodiscard]] static std::unique_ptr<Timeline_Of_Values> timeline_from_str( e_timeline_type_t type, std::string_view str );
	[[nodiscard]] static std::vector<Color> color_list_from_str( std::string_view str );
};

}

