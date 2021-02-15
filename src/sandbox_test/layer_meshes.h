#pragma once

// ----------------------------------------------------------------------------

struct layer_meshes : w_layer
{
	ec_mesh* mec;
	w_tween mesh_rotator;

	layer_meshes();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
