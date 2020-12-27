#pragma once

struct w_layer : i_life_cycle, i_input_receiver
{
	std::vector<std::unique_ptr<w_entity>> entities;
	w_imgui_data_provider* ui_data_provider = nullptr;

	struct
	{
		// if true, this layer completely covers and obscures the layers below it in the stack.
		bool draws_completely_solid : 1;

		// if true, the layers below this one can't receive user input
		bool blocks_further_input : 1;

#ifndef _FINALRELEASE
		// if true, this is the layer where the box2d entities live. as a rule, entities
		// using the physics engine live on the same layer. the engine needs a way of
		// knowing which layer that is so it can draw the debug information correctly.
		bool is_debug_physics_layer : 1;
#endif
	};

	w_layer();
	virtual ~w_layer() = default;

	/*
		called each time the layer is pushed onto the stack

		good for setting state variables and making sure the state
		is ready to start updating/drawing
	*/
	virtual void push() {}

	/*
		called each time the layer is removed from the stack
	*/
	virtual void pop() {}

	/*
		called when this layer is becoming the top layer.

		-	when it is initially pushed onto the stack
		-	when the layer above it is popped off and this
			becomes the topmost layer
	*/
	virtual void becoming_top_layer() {}

	/*
		called when this layer is being getting_covered up by another layer
	*/
	virtual void getting_covered() {}

	virtual void update();

	virtual void draw();
	virtual void draw_ui();

	e_camera* get_camera();

	template<typename T>
	T* add_entity()
	{
		// if this is the first entity being added to the layer, go ahead and allocate space for
		// a bunch - entities don't usually travel alone.
		if( entities.empty() )
		{
			entities.reserve( 32 );
		}

		entities.emplace_back( std::make_unique<T>() );

		auto new_entity = static_cast<T*>( entities.back().get() );
		new_entity->parent_layer = this;

		return new_entity;
	}

	[[nodiscard]] bool is_topmost_layer() const;

	// #optimization - this feels like a bottleneck waiting to happen. some sort of map look up would be way faster.
	//				  - would an MRU lookup table be useful? it gets populated as you look things up and emptied on new_game?

	template<typename T>
	[[nodiscard]] T* find_from_tag( const char* tag )
	{
		for( auto& iter : entities )
		{
			if( iter->tag == tag )
			{
				return static_cast<T*>( iter.get() );
			}
		}

		return nullptr;
	}
};
