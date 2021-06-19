
namespace war
{

struct scene_mgr
{
	// the scenes are stored front-to-back so this means that the topmost scene
	// on the screen is first in the vector. meaning, new scenes are inserted at
	// the front.
	//
	// therefore, iterating forwards through scene_stack is drilling downwards
	// into the screen.

	// #warren - can we kill the unique_ptr here? deque is already using the heap.
	std::deque<std::unique_ptr<scene>> scene_stack;

	// holds a pointer to the current scene being drawn. this is needed to
	// retrieve things like the viewport_pivot as scenes are being drawn,
	// layered on top of each other.
	scene* current_scene = nullptr;

	transform* get_transform();
	vec2 get_viewport_pivot();

	template<typename T>
	void push()
	{
		auto new_scene = std::make_unique<T>();

		if( !scene_stack.empty() )
		{
			scene* top_scene = get_top();

			if( top_scene->life_cycle.is_alive() and typeid( *top_scene ) == typeid( *new_scene ) )
			{
				log_fatal( "Duplicate scene types at top of stack : {}", typeid( *top_scene ).name() );
				assert( false );
			}

			top_scene->getting_covered();
		}

		new_scene->pushed();
		new_scene->becoming_top_scene();

		scene_stack.push_front( std::move( new_scene ) );
	}

	void clear_stack();
	void pop();
	[[nodiscard]] scene* get_top();

	void pre_update();

	void remove_dead_scenes();

	void update();
	void post_update();

	void draw();

	int find_first_fully_opaque_scene();
	void draw_scene( int starting_scene_idx );
	void draw_scene_ui( int starting_scene_idx );

	virtual void new_game();

	bool on_input_motion( const input_event* evt );
	bool on_input_pressed( const input_event* evt );
	bool on_input_held( const input_event* evt );
	bool on_input_released( const input_event* evt );
	bool on_input_key( const input_event* evt );
};

}
