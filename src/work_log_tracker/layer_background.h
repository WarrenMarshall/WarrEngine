#pragma once

struct layer_background : w_layer
{
	a_gradient* background = nullptr;

	layer_background();

	virtual void push() override;
	virtual void draw() override;
};
