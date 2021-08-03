
namespace war
{

struct simple_collision_world
{
	scene* parent_scene = nullptr;

	std::vector<ec_simple_collision_body*> active_bodies;
	//std::vector<simple_collision::pending_collision> pending_collisions;
	//std::vector<simple_collision::pending_collision> pending_touches;
	//std::set<entity*> unique_entities_with_collisions;
	std::set<ec_simple_body_pair> colliding_bodies_set;
	bool need_another_iteration = false;

	simple_collision_world( scene* parent_scene );

	void ray_cast( simple_collision::raycast_callback* callback, const entity* entity, const vec2& start, const vec2& end ) const;

	void generate_collision_set();
	void push_apart( simple_collision::pending_collision& coll );
	void resolve_collision( entity* a, entity* b );
};

}
