#pragma once

struct layer_worldviewport : w_layer
{
	std::unique_ptr<a_image> img_gradient = nullptr;

	void push() override;
	void draw() override;
};
