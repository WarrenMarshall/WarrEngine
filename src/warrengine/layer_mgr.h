#pragma once

struct w_layer_mgr
{
	// the layers are stored front-to-back so this means that the topmost layer
	// on the screen is first in the vector. meaning, new layers are inserted at
	// the front.
	//
	// therefore, iterating forwards through layer_stack is drilling downwards
	// into the screen.

	std::deque<std::unique_ptr<w_layer>> layer_stack;

	template<typename T>
	void push()
	{
		auto new_layer = std::make_unique<T>();

		if( !layer_stack.empty() )
		{
			w_layer* top_layer = get_top();

			if( top_layer->ilc_is_alive() )
			{
				if( typeid( *top_layer ) == typeid( *new_layer ) )
				{
					log_error( "Duplicate layer types at top of stack : {}", typeid( *top_layer ).name() );
					assert( false );
				}
			}

			top_layer->getting_covered();
		}

		new_layer->push();
		new_layer->becoming_top_layer();

		layer_stack.push_front( std::move( new_layer ) );
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
	bool on_input_key( const w_input_event* evt );
};