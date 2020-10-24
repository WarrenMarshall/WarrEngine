#pragma once

struct layer_default : w_layer
{
	a_gradient* gradient = nullptr;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
};
