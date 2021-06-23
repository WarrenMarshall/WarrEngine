
namespace war::simple_collision
{

struct world
{
	vec2 gravity = {};

	world() = default;
	world( vec2 gravity );
	void ray_cast( raycast_callback* callback, const vec2& start, const vec2& end ) const;
};

}
