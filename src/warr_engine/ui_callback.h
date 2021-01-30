#pragma once

// callback objects for the UI system are a catch all for telling
// the system what we want to do with events, or which graphics to use,
// or what the state of data is.
//
// this is how the UI system talks to the outside world.

struct w_imgui_callback
{
	virtual w_imgui_control_data get_data_for_control(const w_imgui_control& control);
	virtual a_texture* get_texture_for_checkbox( const w_imgui_control& control );

	virtual void on_left_clicked( const w_imgui_control& control, const w_imgui_result& result );
	virtual void on_motion( const w_imgui_control& control, const w_imgui_result& result );

	virtual float get_control_padding();
	virtual float get_control_margin();
	virtual float get_default_width( const w_imgui_control& control );
	virtual float get_default_width( e_imgui_control_type control_type );
	virtual float get_default_height( const w_imgui_control& control );
	virtual float get_default_height( e_imgui_control_type control_type );

	virtual bool on_input_motion( const w_input_event* evt );
	virtual bool on_input_pressed( const w_input_event* evt );
	virtual bool on_input_held( const w_input_event* evt );
	virtual bool on_input_released( const w_input_event* evt );
	virtual bool on_input_key( const w_input_event* evt );

	// if a control is meant to retain focus across frames, store the tag here.
	hash tag_focus = hash_none;
};
