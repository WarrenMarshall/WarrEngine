#pragma once

struct layer_default : w_layer
{
	layer_default();

	virtual void push() final;
	virtual void draw() final;
};
