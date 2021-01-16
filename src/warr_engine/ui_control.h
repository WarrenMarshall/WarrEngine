#pragma once

struct w_imgui_control
{
	e_imgui_control_type type = imgui_control_type::none;
	bool is_active = true;
	hash tag;
	std::string text;
	e_align text_align = align::hcenter | align::vcenter;
	a_9slice_def* slice_def = nullptr;
	e_align texture_align = align::fill;

	w_rect rc_win = { 0,0,0,0 };
	w_rect rc_client = { 0,0,0,0 };
};
