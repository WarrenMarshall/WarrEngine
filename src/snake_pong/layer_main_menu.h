#pragma once

// ----------------------------------------------------------------------------

struct layer_main_menu_ui_callback : w_imgui_callback
{
	virtual void was_left_clicked( w_imgui_control* control ) override;
};

// ----------------------------------------------------------------------------

struct layer_main_menu : w_layer
{
	layer_main_menu_ui_callback imgui_callback;

	layer_main_menu();

	virtual void draw_ui() override;
	virtual void becoming_top_layer() override;
	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;

	virtual w_imgui_callback* get_imgui_callback() override;

	virtual bool iir_on_pressed( const w_input_event* evt ) override;
	virtual bool iir_on_held( const w_input_event* evt ) override;
	virtual bool iir_on_motion( const w_input_event* evt ) override;
};
