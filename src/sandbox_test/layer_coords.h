#pragma once

// ----------------------------------------------------------------------------

struct layer_coords_callback : w_imgui_callback
{
	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result ) override;
	virtual void on_motion( const w_imgui_control& control, const w_imgui_result& result ) override;
};

// ----------------------------------------------------------------------------

struct layer_coords : w_layer
{
	layer_coords_callback imgui_callback;

	layer_coords();

	virtual void push() override;
	virtual void draw() override;
	virtual void draw_ui() override;
	virtual w_imgui_callback* get_imgui_callback() override;

	virtual bool on_input_motion( const w_input_event* evt ) override;
};
