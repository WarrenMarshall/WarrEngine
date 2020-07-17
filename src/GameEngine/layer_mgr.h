#pragma once

struct w_layer_mgr : i_listener
{
	/*
		the layers are stored front-to-back

		this means that the topmost layer is first
		in the list.

		so iterating forwards from the start of the deque
		is drilling downwards into the screen.
	*/
	std::deque<std::unique_ptr<w_layer>> layer_stack;

	void push( std::unique_ptr<w_layer> layer );
	void pop();
	w_layer* get_top();

	int get_topmost_opaque_idx();
	void update();
	void draw();

	w_layer* find_topmost_input_listener();
	void on_listener_event_received( e_event_id event, void* object ) override;
};
