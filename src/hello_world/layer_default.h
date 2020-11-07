#pragma once

struct layer_default : w_layer
{
	a_gradient* gradient = nullptr;
	a_subtexture* subtex = nullptr;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
