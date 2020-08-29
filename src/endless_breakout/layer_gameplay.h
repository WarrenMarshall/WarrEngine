#pragma once

struct layer_gameplay : w_layer
{
	layer_gameplay();

	virtual void push() override;
	virtual void draw() override;

	virtual bool handle_input_event( const w_input_event* evt ) override;
};
