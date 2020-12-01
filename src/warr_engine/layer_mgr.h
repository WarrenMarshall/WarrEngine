#pragma once

struct w_layer_mgr : i_input_receiver
{
	/*
		the layers are stored front-to-back

		this means that the topmost layer is first in the list.

		so iterating forwards is drilling downwards into the screen.
	*/
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

	virtual bool iir_on_motion( const w_input_event* evt ) override;
	virtual bool iir_on_pressed( const w_input_event* evt ) override;
	virtual bool iir_on_held( const w_input_event* evt ) override;
	virtual bool iir_on_released( const w_input_event* evt ) override;
};
