#pragma once

// ----------------------------------------------------------------------------

struct layer_sound_board_ui_callback : w_imgui_callback
{
	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;
};

// ----------------------------------------------------------------------------

struct layer_sound_board : w_layer
{
	a_texture* gradient = nullptr;

	layer_sound_board_ui_callback imgui_callback;

	layer_sound_board();

	virtual void draw() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
