#pragma once

struct w_layer : i_life_cycle
{
	std::vector<std::unique_ptr<w_entity>> entities;
	w_imgui_callback* imgui_callback = nullptr;

	// if true, this layer completely covers and obscures the layers below it in the stack.
	bool draws_completely_solid = false;

	// if true, the layers below this one can't receive user input
	bool blocks_further_input = false;

#ifndef _FINALRELEASE
	// if true, this is the layer where the box2d entities live. as a rule, entities
	// using the physics engine live on the same layer. the engine needs a way of
	// knowing which layer that is so it can draw the debug information correctly.
	bool is_debug_physics_layer = false;
#endif

	w_layer();
	virtual ~w_layer() = default;

	// called each time the layer is pushed onto the stack
	//
	// good for setting state variables and making sure the state is ready to
	// start updating/drawing

	virtual void push() {}

	// called each time the layer is removed from the stack

	virtual void pop() {}

	// called when this layer is becoming the top layer.
	//
	// - when it is initially pushed onto the stack
	// - when the layer above it is popped off and this becomes the topmost
	//   layer

	virtual void becoming_top_layer() {}

	// called when this layer is being getting_covered up by another layer

	virtual void getting_covered() {}

	virtual void update();

	virtual void draw();
	virtual void draw_ui();

	w_entity* get_camera();

	template<typename T>
	T* add_entity()
	{
		entities.emplace_back( std::make_unique<T>() );
		auto new_entity = static_cast<T*>( entities.back().get() );
		return new_entity;
	}

	[[nodiscard]] bool is_topmost_layer() const;
	[[nodiscard]] w_entity* find_entity( hash tag );

	virtual void new_game();
	virtual w_imgui_callback* get_imgui_callback();

	virtual bool on_input_motion( const w_input_event* evt );
	virtual bool on_input_pressed( const w_input_event* evt );
	virtual bool on_input_held( const w_input_event* evt );
	virtual bool on_input_released( const w_input_event* evt );
	virtual bool on_input_key( const w_input_event* evt );
};
