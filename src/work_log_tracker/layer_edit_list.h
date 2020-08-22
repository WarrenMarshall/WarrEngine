#pragma once

struct layer_edit_list : w_layer
{
	std::unique_ptr<w_ui_style_button> style_button = nullptr;
	std::unique_ptr<w_ui_style_button> style_approved_button = nullptr;
	std::unique_ptr<w_ui_style_button> style_clear_button = nullptr;
	w_entity* test_entity = nullptr;

	void push() final;
	void draw() final;
	virtual bool handle_input_event( const w_input_event* evt ) override;
};
