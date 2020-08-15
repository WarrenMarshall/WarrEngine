#pragma once

struct GE_API w_layer_mgr : i_listener
{
	/*
		the layers are stored front-to-back

		this means that the topmost layer is first in the list.

		so iterating forwards is drilling downwards into the screen.
	*/
	std::vector<std::shared_ptr<w_layer>> layer_stack;

	void push( std::shared_ptr<w_layer> layer );
	void pop();
	w_layer* get_top();

	void update();
	void draw();

	void on_listener_event_received( e_event_id event, void* object ) override;
};
