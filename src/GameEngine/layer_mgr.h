#pragma once

struct w_layer_mgr
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

	int get_opaque_index( e_opaque opaque_flags );
	void update();
	void draw();
};
