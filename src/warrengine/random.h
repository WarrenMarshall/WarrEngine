
namespace war
{

struct Random
{
	static void seed();
	[[nodiscard]] static float getf();
	[[nodiscard]] static bool getb();
	[[nodiscard]] static float getf_range( float min, float max );
	[[nodiscard]] static int32_t geti_range( int32_t min, int32_t max );
	[[nodiscard]] static Vec2 get_random_in_circle( float radius = 1.0f );
	[[nodiscard]] static Vec2 get_random_on_circle( float radius = 1.0f );
	[[nodiscard]] static Vec2 get_random_unit_vector();
};

}
