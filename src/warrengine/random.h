
namespace war
{

struct random
{
	static void seed();
	[[nodiscard]] static float getf();
	[[nodiscard]] static bool getb();
	[[nodiscard]] static float getf_range( float min, float max );
	[[nodiscard]] static int geti_range( int min, int max );
	[[nodiscard]] static vec2 get_random_in_circle( float radius = 1.0f );
	[[nodiscard]] static vec2 get_random_on_circle( float radius = 1.0f );
	[[nodiscard]] static vec2 get_random_unit_vector();
};

}
