#pragma once

struct layer_gameviewport : w_layer
{
	std::unique_ptr<a_image> img_gradient = nullptr;

	virtual void push();
	virtual void draw();
};
