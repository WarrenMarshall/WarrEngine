
namespace war
{

struct Random final
{
	static void seed();
	[[nodiscard]] static f32 getf();
	[[nodiscard]] static bool getb();
	[[nodiscard]] static f32 getf_range( f32 min, f32 max );
	[[nodiscard]] static i32 geti_range( i32 min, i32 max );
	[[nodiscard]] static Vec2 get_random_in_circle( f32 radius = 1.0f );
	[[nodiscard]] static Vec2 get_random_on_circle( f32 radius = 1.0f );
	[[nodiscard]] static Vec2 get_random_unit_vector();
};

}
