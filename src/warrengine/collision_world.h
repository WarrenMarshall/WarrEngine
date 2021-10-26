
namespace war
{

struct Collision_World final
{
	Scene* parent_scene = nullptr;

	struct
	{
		// how far 2 bodies need to be penetrating before we will push them apart.
		// setting this higher results in a better dynamic simulation but can look
		// choppy in a platformer where the character is constantly being pushed
		// down by gravity.
		//
		// 0.25 - good for a platformer
		// 2.0 - good for dynamically moving bodies bouncing off each other

		f32 push_apart_tolerance = 0.25f;

		// we allow for a few iterations here to do our best to free colliders from
		// other colliders. if this ends up being slow, adjust this value downwards.
		//
		// a low number works well here for a small number of things pushing
		// against each other. a larger number is needed when you want to have,
		// say, an entity pushing many other entities at the same time. low
		// values lead to some bodies penetrating each other while force is
		// being applied but then correct when you let go. basically, the higher
		// this value the more solid objects will appear at a perf cost.
		//
		// 3 = good enough for platforming and most games
		// 20 = pushing 10+ entities at a time

		size_t max_pos_iterations = 3;

		// collisions do better if you can add a little padding between collision bodies
		// after resolution. 1.0 would be exactly snug against each other. Anything
		// above 1.0 represents the skin thickness we are adding.
		f32 skin_thickness = 1.1f;

		// the maximum speed that we allow things to fall when applying gravity
		f32 gravity_max_speed = collision_gravity_default * 3.0f;

	} settings;

	std::vector<Collision_Body_Component*> active_bodies;
	bool need_another_iteration = false;

	Collision_World() = default;
	Collision_World( Scene* parent_scene );

	void ray_cast( collision::Raycast_Callback* callback, const Entity* entity, const Vec2& start, const Vec2& end ) const;

	void handle_collisions();
	void push_apart( collision::Pending_Collision& coll );
	void resolve_solid_collision( collision::Pending_Collision& coll );
	void resolve_sensor_collision( collision::Pending_Collision& coll );
	void init_sensor_sets_for_new_frame() const;
	void process_sensor_sets() const;
};

}
