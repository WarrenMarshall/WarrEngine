#pragma once

// ----------------------------------------------------------------------------

struct layer_particles : w_layer
{
	layer_particles();

	w_imgui_control_data cb_follow_mouse = false;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
