#pragma once

struct layer_particles : w_layer
{
	layer_particles();

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
	virtual void draw_ui() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
