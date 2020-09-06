#pragma once

struct layer_default : w_layer
{
	layer_default();

	w_entity* test_entity = nullptr;

	virtual void push() final;
	virtual void draw() final;

	virtual bool handle_input_event( const w_input_event* evt );
};
