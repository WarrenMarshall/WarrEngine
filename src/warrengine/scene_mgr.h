
namespace war
{

struct Scene_Mgr final
{
	Timeline_Of_Nodes transition_timeline;

	// the scenes are stored front-to-back so this means that the topmost scene
	// on the screen is first in the vector. meaning, new scenes are inserted at
	// the front.
	//
	// therefore, iterating forwards through scene_stack is drilling downwards
	// into the screen.

	std::vector<std::unique_ptr<Scene>> scene_stack;

	// holds a pointer to the current scene being drawn. this is needed to
	// retrieve things like the viewport_pivot as scenes are being drawn,
	// layered on top of each other.
	Scene* current_scene = nullptr;

	[[nodiscard]] Transform* get_transform()
	{
		return current_scene->get_transform();
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

	Vec2 get_viewport_pivot();

	void do_transient_timeline( Timeline_Of_Nodes timeline )
	{
		assert( transition_timeline.life_cycle.is_dead() );

		transition_timeline = timeline;
		push<Scene_Transient>();
		transition_timeline.restart();
	}

	// pushes a new scene underneath the current one on top. this is most useful
	// when a scene_dummy is at the top of the stack and you want to insert a
	// new scene below it so THAT scene will be on top once the dummy clears.

	void push_under( Scene* new_scene )
	{
		if( scene_stack.empty() )
		{
			log_fatal( "scene stack is empty" );
		}

		Scene* top_scene = get_top();

		if( top_scene->life_cycle.is_alive() and typeid( *top_scene ) == typeid( *new_scene ) )
		{
			log_fatal( "Duplicate scene types at top of stack : {}", typeid( *top_scene ).name() );
			assert( false );
		}

		new_scene->pushed();
		std::unique_ptr<Scene> fuck;
		fuck.reset( new_scene );
		scene_stack.insert( scene_stack.begin() + 1, std::move( fuck ) );

	}

	// pushes a new scene on top of the stack
	template<typename T>
	void push()
	{
		auto new_scene = std::make_unique<T>();

		if( !scene_stack.empty() )
		{
			Scene* top_scene = get_top();

			if( top_scene->life_cycle.is_alive() and typeid( *top_scene ) == typeid( *new_scene ) )
			{
				log_fatal( "Duplicate scene types at top of stack : {}", typeid( *top_scene ).name() );
				assert( false );
			}

			top_scene->getting_covered();
		}

		new_scene->pushed();
		new_scene->becoming_top_scene();

		scene_stack.insert( scene_stack.begin(), std::move( new_scene ) );
	}

	void clear_stack();
	void pop();
	void pop_under();
	[[nodiscard]] Scene* get_top();
	[[nodiscard]] Scene* get_under();

	void pre_update();
	void update();
	void post_update();

	void draw();

	void draw_scene_stack( i32 starting_scene_idx );
	void draw_scene_stack_ui( i32 starting_scene_idx );

	virtual void new_game();

	bool on_input_motion( const Input_Event* evt );
	bool on_input_pressed( const Input_Event* evt );
	bool on_input_held( const Input_Event* evt );
	bool on_input_released( const Input_Event* evt );
	bool on_input_key( const Input_Event* evt );
};

}
