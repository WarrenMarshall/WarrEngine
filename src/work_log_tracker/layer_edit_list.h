#pragma once

struct layer_edit_list : w_layer
{
	std::unique_ptr<w_ui_style_button> style_button = nullptr;
	w_entity* test_entity = nullptr;

	layer_edit_list();

	void push() final;
	void draw() final;
	virtual bool handle_input_event( const w_input_event* evt ) override;
};
