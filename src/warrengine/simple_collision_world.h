
namespace war
{

struct simple_collision_world
{
	scene* parent_scene = nullptr;

	// how far 2 bodies need to be penetrating before we will push them apart.
	// setting this higher results in a better dynamic simulation but can look
	// choppy in a platformer where the character is constantly being pushed
	// down by gravity.
	//
	// 0.25 - good for a platformer
	// 2.0 - good for dynamically moving bodies bouncing off each other

	float push_apart_tolerance = 0.25f;

	std::vector<ec_simple_collision_body*> active_bodies;
	std::set<ec_simple_body_pair> colliding_bodies_set;
	bool need_another_iteration = false;

	simple_collision_world( scene* parent_scene );

	void ray_cast( simple_collision::raycast_callback* callback, const entity* entity, const vec2& start, const vec2& end ) const;

	void handle_collisions();
	void push_apart( simple_collision::pending_collision& coll );
	void resolve_collision( simple_collision::pending_collision& coll );
	void resolve_touch( simple_collision::pending_collision& coll );
};

}
