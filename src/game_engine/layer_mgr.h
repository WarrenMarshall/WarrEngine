#pragma once

struct w_layer_mgr : i_listener
{
	/*
		the layers are stored front-to-back

		this means that the topmost layer is first in the list.

		so iterating forwards is drilling downwards into the screen.
	*/
	std::vector<std::unique_ptr<w_layer>> layer_stack;

	template<typename T>
	void push()
	{
		auto new_layer = std::make_unique<T>();

		w_layer* top_layer = get_top();
		if( top_layer )
		{
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

	void pop();
	w_layer* get_top();

	void update();
	void draw();

	virtual void on_listener_event_received( e_event_id event, void* object ) override;
};
