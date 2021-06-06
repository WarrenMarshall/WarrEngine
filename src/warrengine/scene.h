
namespace war
{

struct scene
{
	life_cycle_mgr life_cycle;

	void save_mouse_mode();
	void restore_mouse_mode();
	std::optional<e_mouse_mode> saved_mouse_mode = std::nullopt;

	// scenes can have a camera attached to them. this is applied against the
	// view matrix.
	transform* get_transform();
	transform camera_transform;

	// rendering starts in the top left corner of the screen by default. this
	// provides a way to move that anywhere within the viewport using a simple
	// offset.
	//
	// note : this is NOT a camera. this is purely a viewport offset.

	vec2 get_viewport_pivot();
	vec2 viewport_pivot = { viewport_hw - 1.f, viewport_hh - 1.f };

	std::deque<std::unique_ptr<entity>> entities;

	// if set to anything other than hash_none, some control is in it's expanded
	// state. this means that we don't want mouse input going to other controls
	// while that expanded state is active.
	hash ui_expanded_tag_begin = hash_none;
	hash ui_expanded_tag_end = hash_none;

	// set this flag to true, by calling force_close_expanded_controls(), to collapse
	// any expanded UI controls that are waiting for the user to do something.
	// this is a handy way to close out things like dropdown controls without
	// having to actually click on them.

	bool clear_expanded_tag_this_frame = false;
	void force_close_expanded_controls();

	// if true, this scene completely covers and obscures the scenes below it in the stack.
	bool draws_completely_solid = false;

	// if true, the scenes below this one can't receive user input
	bool blocks_further_input = false;

	// if true, layers below this one will NOT get their Update() functions called
	bool blocks_further_update = false;

	// if true, this is the scene where the box2d entities live. as a rule, entities
	// using the physics engine live on the same scene. the engine needs a way of
	// knowing which scene that is so it can draw the debug information correctly.
	bool is_debug_physics_scene = false;

	scene();
	virtual ~scene() = default;

	// ----------------------------------------------------------------------------
	// entity selections

	void select_by_pick_id( int pick_id );
	void deselect_all();
	std::vector<entity*> get_selected();

	// ----------------------------------------------------------------------------

	// called each time the scene is pushed onto the stack
	//
	// good for setting state variables and making sure the state is ready to
	// start updating/drawing

	virtual void pushed()
	{
	}

	// called each time the scene is removed from the stack

	virtual void popped()
	{
	}

	// called when this scene is becoming the top scene.
	//
	// - when it is initially pushed onto the stack
	// - when the scene above it is popped off and this becomes the topmost
	//   scene

	virtual void becoming_top_scene()
	{
	}

	// called when this scene is being getting_covered up by another scene

	virtual void getting_covered()
	{
	}

	virtual void pre_update();
	virtual void update();
	virtual void post_update();

	virtual void draw();
	virtual void draw_ui();

	template<typename T>
	T* add_entity()
	{
		entities.emplace_back( std::make_unique<T>() );
		auto new_entity = static_cast<T*>( entities.back().get() );
		return new_entity;
	}

	[[nodiscard]] bool is_topmost_scene() const;
	[[nodiscard]] entity* find_entity( hash tag );
	[[nodiscard]] entity* find_entity_by_pick_id( int pick_id );

	virtual void new_game();

	std::unique_ptr<war::ui_callback> ui_callback = nullptr;
	virtual war::ui_callback* get_ui_callback();

	virtual bool on_input_motion( const input_event* evt );
	virtual bool on_input_pressed( const input_event* evt );
	virtual bool on_input_held( const input_event* evt );
	virtual bool on_input_released( const input_event* evt );
	virtual bool on_input_key( const input_event* evt );
};

}