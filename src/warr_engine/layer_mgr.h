#pragma once

struct w_layer_mgr
{
	// the layers are stored front-to-back so this means that the topmost layer
	// on the screen is first in the vector.
	//
	// therefore, iterating forwards through this vector is drilling downwards
	// into the screen.

	std::vector<std::unique_ptr<w_layer>> layer_stack;

	~w_layer_mgr();

	template<typename T>
	void push()
	{
		auto new_layer = std::make_unique<T>();

		if( !layer_stack.empty() )
		{
			w_layer* top_layer = get_top();

			if( typeid( *top_layer ) == typeid( *new_layer ) )
			{
				log_error( "Duplicate layer types at top of stack : {}", typeid( *top_layer ).name() );
				assert( false );
			}

			top_layer->getting_covered();
		}

		new_layer->push();
		new_layer->becoming_top_layer();

		layer_stack.insert( layer_stack.begin(), std::move( new_layer ) );
	}

	void clear_stack();
	void pop();
	[[nodiscard]] w_layer* get_top();

	void update();
	void draw();
	virtual void new_game();

	bool on_input_motion( const w_input_event* evt );
	bool on_input_pressed( const w_input_event* evt );
	bool on_input_held( const w_input_event* evt );
	bool on_input_released( const w_input_event* evt );
};
