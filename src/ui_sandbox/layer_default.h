#pragma once

struct layer_default : w_layer
{
	a_gradient* gradient = nullptr;
	float angle = 0.0f;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual void update() override;
};
