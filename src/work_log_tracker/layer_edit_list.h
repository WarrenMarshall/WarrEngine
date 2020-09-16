#pragma once

struct layer_edit_list : w_layer
{
	std::unique_ptr<w_ui_style_button> style_button = nullptr;
	std::unique_ptr<w_ui_style_button> style_approved_button = nullptr;
	std::unique_ptr<w_ui_style_button> style_waiting_button = nullptr;
	std::unique_ptr<w_ui_style_button> style_clear_button = nullptr;

	virtual void push() override;
	virtual void draw() override;
};
