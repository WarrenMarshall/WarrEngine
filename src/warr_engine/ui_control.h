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
	w_range value_range = { 0.0f, 100.0f };
	float value = 0.0f;

	w_rect rc_win = { 0,0,0,0 };
	w_rect rc_client = { 0,0,0,0 };

	bool is_active = true;
	bool set_position_called = false;
};
