#pragma once

// ----------------------------------------------------------------------------

struct layer_entity_picking : w_layer
{
	layer_entity_picking();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
