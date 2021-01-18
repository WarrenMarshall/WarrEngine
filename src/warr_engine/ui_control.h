#pragma once

struct w_imgui_control
{
	e_imgui_control_type type = imgui_control_type::none;
	// the ui control that this control lives inside of
	hash tag_container;
	hash tag;
	std::string text;
	e_align text_align = align::hcenter | align::vcenter;
	a_9slice_def* slice_def = nullptr;
	e_align texture_align = align::fill;

	w_rect rc_win = { 0,0,0,0 };
	w_rect rc_client = { 0,0,0,0 };

	// is this control an "active" or "passive" control?
	bool is_active = true;

	// has the "set_position" call been made for this control? used when deciding
	// whether or not to use default sizes.
	bool set_position_called = false;

	// when the control is hot, does it want callbacks with active mouse positions?
	bool wants_motion_updates = false;
};
