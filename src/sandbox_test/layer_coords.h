#pragma once

// ----------------------------------------------------------------------------

struct layer_coords : w_layer
{
	layer_coords();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
