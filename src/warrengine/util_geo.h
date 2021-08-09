namespace war
{

struct util_geo
{
	[[nodiscard]] static std::vector<vec2> generate_convex_shape( int sides, float radius );
};

}
