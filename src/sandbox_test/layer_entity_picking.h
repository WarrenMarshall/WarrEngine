#pragma once

// ----------------------------------------------------------------------------

struct layer_entity_picking : w_layer
{
	layer_entity_picking();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_pressed( const w_input_event* evt ) override;
};
