#pragma once

struct w_imgui_control
{
	e_imgui_control_type type = imgui_control_type::none;
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
	bool set_position_called =  false;

	// indicates that a control, once hot, won't release it's hover state until
	// the mouse button is actually released.
	//
	// used for controls that want continuous feedback, like sliders, so they
	// don't hop around between hover states as you drag.
	bool sticky_hover = false;

	// does this control do a position offset when it's made hot / clicked?
	bool uses_click_offset = true;
};
