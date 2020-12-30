#pragma once

struct layer_background : w_layer
{
	//a_gradient* gradient = nullptr;

	layer_background();

	virtual void push() override;
	virtual void draw() override;
};
