#pragma once

// callback objects for the UI system are a catch all for telling
// the system what we want to do with events, or which graphics to use,
// or what the state of data is.
//
// this is how the UI system talks to the outside world.

struct w_imgui_callback
{
	virtual e_imgui_control_state get_state_for_control( w_imgui_control* control );
	virtual void was_left_clicked( w_imgui_control* control );
	virtual void was_right_clicked( w_imgui_control* control );
};
