
namespace war
{

struct Random
{
	static void seed();
	[[nodiscard]] static float_t getf();
	[[nodiscard]] static bool getb();
	[[nodiscard]] static float_t getf_range( float_t min, float_t max );
	[[nodiscard]] static int32_t geti_range( int32_t min, int32_t max );
	[[nodiscard]] static Vec2 get_random_in_circle( float_t radius = 1.0f );
	[[nodiscard]] static Vec2 get_random_on_circle( float_t radius = 1.0f );
	[[nodiscard]] static Vec2 get_random_unit_vector();
};

}
