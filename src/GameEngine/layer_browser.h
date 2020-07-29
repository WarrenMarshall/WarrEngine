#pragma once

struct layer_browser : w_layer
{
	a_gradient* grad_overlay = nullptr;

	void push() override;
	void pop() override;
	void becoming_top_layer() override;
	void draw() override;
};
