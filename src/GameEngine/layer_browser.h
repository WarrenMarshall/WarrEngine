#pragma once

struct layer_browser : w_layer
{
	a_gradient* grad_overlay = nullptr;

	std::unique_ptr<w_ui_style_pushbutton> style_close_button = nullptr;
	std::unique_ptr<w_ui_style_panel> style_panel = nullptr;
	std::unique_ptr<w_ui_style_tile> style_tile = nullptr;

	void push() override;
	void pop() override;
	void becoming_top_layer() override;
	void draw() override;
};
