#pragma once

struct layer_background : w_layer
{
	a_texture* gradient = nullptr;

	layer_background();

	virtual void push() override;
	virtual void draw() override;
};
