
namespace war
{

struct simple_collision_world
{
	scene* parent_scene = nullptr;

	std::vector<ec_simple_collision_body*> bodies;
	std::vector<simple_collision::pending_collision> pending_collisions;
	std::vector<simple_collision::pending_collision> pending_touches;

	simple_collision_world( scene* parent_scene );

	void pre_update();
	void update();

	void ray_cast( simple_collision::raycast_callback* callback, const entity* entity, const vec2& start, const vec2& end ) const;
};

}
