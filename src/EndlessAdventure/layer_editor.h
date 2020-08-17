#pragma once

struct layer_editor : w_layer
{
	w_rect tile_display_area;
	a_gradient* gradient = nullptr;

	std::unique_ptr<w_ui_style_button> style_browse = nullptr;
	std::unique_ptr<w_ui_style_button> style_arrow_button = nullptr;
	std::unique_ptr<w_ui_style_panel> style_panel = nullptr;
	std::unique_ptr<w_ui_style_panel> style_thin_panel = nullptr;
	std::unique_ptr<w_ui_style_tile> style_tile = nullptr;

	bool is_painting = false;

	void push() override;
	void pop() override;
	void becoming_top_layer() override;
	void draw() override;
	bool handle_input_event( const w_input_event* evt ) override;
};
