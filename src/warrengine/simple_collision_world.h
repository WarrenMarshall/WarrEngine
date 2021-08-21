
namespace war
{

struct simple_collision_world
{
	scene* parent_scene = nullptr;

	struct
	{
		// how far 2 bodies need to be penetrating before we will push them apart.
		// setting this higher results in a better dynamic simulation but can look
		// choppy in a platformer where the character is constantly being pushed
		// down by gravity.
		//
		// 0.25 - good for a platformer
		// 2.0 - good for dynamically moving bodies bouncing off each other

		float push_apart_tolerance = 0.25f;

		// we allow for a few iterations here to do our best to free colliders from
		// other colliders. if this ends up being slow, adjust this value downwards.

		size_t num_pos_iterations = 20;

		// collisions do better if you can add a little padding between collision bodies
		// after resolution. 1.0 would be exactly snug against each other. Anything
		// above 1.0 represents the skin thickness we are adding.
		float skin_thickness = 1.1f;

		// the maximum speed that we allow things to fall when applying gravity
		float gravity_max_speed = simple_collision_gravity_default * 3.0f;

	} settings;

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
