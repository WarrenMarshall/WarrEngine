
namespace war
{

struct Scene
{
	Quad_Tree spatial_map;
	Life_Cycle life_cycle;

	// scenes can have a camera attached to them. this is applied against the
	// view matrix.
	[[nodiscard]] Transform* get_transform()
	{
		return &camera_transform;
	}

	[[nodiscard]] Vec2 get_pos()
	{
		return get_transform()->pos;
	}
	[[nodiscard]] f32 get_angle()
	{
		return get_transform()->angle;
	}
	[[nodiscard]] f32 get_scale()
	{
		return get_transform()->scale;
	}

	// rendering starts in the top left corner of the screen by default. this
	// provides a way to move that anywhere within the viewport using a simple
	// offset.
	//
	// note : this is NOT a camera. this is purely a viewport offset.

	Vec2 get_viewport_pivot();
	Vec2 viewport_pivot = { viewport_hw - 1.f, viewport_hh - 1.f };

	std::vector<std::unique_ptr<Entity>> entities;

	Collision_World sc_world;

	// if set to anything other than hash_none, some control is in it's expanded
	// state. this means that we don't want mouse input going to other controls
	// while that expanded state is active.
	hash ui_expanded_tag_begin = hash_none;
	hash ui_expanded_tag_end = hash_none;

	void force_close_expanded_controls();

	struct
	{
		// if true, this scene completely covers and obscures the scenes below it in the stack.
		bool blocks_further_drawing : 1 = false;

		// if true, the scenes below this one can't receive user input
		bool blocks_further_input : 1 = false;

		// if true, layers below this one will NOT get their Update() functions called
		bool blocks_further_update : 1 = false;

		// if true, this is the scene where the box2d entities live. as a rule, entities
		// using the physics engine live on the same scene. the engine needs a way of
		// knowing which scene that is so it can draw the debug information correctly.
		bool is_debug_physics_scene : 1 = false;

		// set this flag to true, by calling force_close_expanded_controls(), to collapse
		// any expanded UI controls that are waiting for the user to do something.
		// this is a handy way to close out things like dropdown controls without
		// having to actually click on them.
		bool clear_expanded_tag_this_frame : 1 = false;

		// if true, this game requires a controller to play. if a controller is
		// not connected, the "controller required" scene will be displayed
		// automatically whenever the engine can't find a connected controller.
		bool requires_controller : 1 = false;

		// if true, this scene represents the main menu of the game. we use this
		// to determine if we should display the "Main Menu" button on the ESC
		// dialog box.
		bool is_main_menu : 1 = false;
	} flags;

	Scene();
	virtual ~Scene() = default;

	// ----------------------------------------------------------------------------
	// entity selections

	void select_by_pick_id( i32 pick_id );
	void deselect_all();
	std::vector<Entity*> get_selected();

	std::set<Entity*> selected_entities;

	// ----------------------------------------------------------------------------

	// called each time the scene is pushed onto the stack
	//
	// good for setting state variables and making sure the state is ready to
	// start updating/drawing

	virtual void pushed();

	// called each time the scene is removed from the stack

	virtual void popped();

	// called when this scene is becoming the top scene.
	//
	// - when it is initially pushed onto the stack
	// - when the scene above it is popped off and this becomes the topmost
	//   scene

	virtual void becoming_top_scene();

	// called when this scene is being getting_covered up by another scene

	virtual void getting_covered();

	virtual void pre_update();
	virtual void update();
	virtual void post_update();

	virtual void draw();
	virtual void draw_ui();

	template<typename T>
	T* add_entity()
	{
		entities.push_back( std::make_unique<T>() );
		auto e = (T*)entities.back().get();
		e->parent_scene = this;
		return e;
	}

	[[nodiscard]] bool is_topmost_scene() const;
	[[nodiscard]] Entity* find_entity( hash tag );
	[[nodiscard]] Entity* find_entity_by_pick_id( i32 pick_id );

	virtual void new_game();
	void follow_cam( const Transform* follow_target );

	UI_Callback* ui_callback = nullptr;
	virtual war::UI_Callback* get_ui_callback();

	virtual bool on_input_motion( const Input_Event* evt );
	virtual bool on_input_pressed( const Input_Event* evt );
	virtual bool on_input_held( const Input_Event* evt );
	virtual bool on_input_released( const Input_Event* evt );
	virtual bool on_input_key( const Input_Event* evt );

	// called every time an entity in this scene registers a touch with a sensor
	// in this scene. this allows the scene to be the main handler of triggers
	// and related events.
	virtual bool on_entity_and_sensor_touching_begin( Entity* entity, Collision_Body_Component* sensor );
	virtual bool on_entity_and_sensor_touching( Entity* entity, Collision_Body_Component* sensor );
	virtual bool on_entity_and_sensor_touching_end( Entity* entity, Collision_Body_Component* sensor );

	// called whenever 2 entities in this scene collide with each other
	virtual bool on_entity_collided_with_entity( Entity* entity_a, Entity* entity_b, collision::Pending_Collision& coll );

private:
	Transform camera_transform;
};

}