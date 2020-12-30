#pragma once

struct layer_default : w_layer
{
	a_texture* gradient = nullptr;
	a_texture* tex_hello_world = nullptr;

	layer_default();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
};
