#pragma once

struct layer_background : w_layer
{
	a_gradient* background_gradient = nullptr;

	layer_background();

	virtual void push() final;
	virtual void draw() final;
};
