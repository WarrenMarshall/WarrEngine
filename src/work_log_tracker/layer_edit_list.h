#pragma once

struct layer_edit_list : w_layer
{
	std::unique_ptr<w_ui_style_button> style_button = nullptr;

	layer_edit_list();

	void push() final;
	void draw() final;
};
