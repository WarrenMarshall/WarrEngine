
#pragma once

// ----------------------------------------------------------------------------

struct layer_esc_menu_ui_callback : w_imgui_callback
{
	virtual void was_left_clicked( const w_imgui_control& control ) override;
};

// ----------------------------------------------------------------------------

struct layer_esc_menu : w_layer
{
	e_mouse_mode save_mouse_mode;

	layer_esc_menu_ui_callback ui_callback;

	virtual void push() override;
	virtual void pop() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;
};
