#pragma once

// ----------------------------------------------------------------------------

struct layer_default : w_layer
{
	a_texture* gradient = nullptr;
	a_texture* tex_hello_world = nullptr;
	w_tween movement_tween, scale_tween;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
};
