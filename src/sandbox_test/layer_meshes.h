#pragma once

// ----------------------------------------------------------------------------

struct layer_meshes : w_layer
{
	ec_mesh* mesh_mario;
	ec_mesh* mesh_crate;

	w_tween mesh_rotator;
	w_tween mesh_scaler;

	layer_meshes();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
