#pragma once

struct w_imgui_control
{
	e_imgui_control_type type = imgui_control_type::none;
	bool is_active = true;
	const char* tag = nullptr;
	std::string label;
	e_align label_align = align::centered;
	a_9slice_def* slice_def = nullptr;
	std::array<a_texture*, imgui_control_state::max> subtextures;
	//a_subtexture* subtexture[ 2 ];
	e_align subtexture_align = align::fill;
	w_rect rc = { 0,0,0,0 };	// full rectangle
	w_rect crc = { 0,0,0,0 };	// client rectangle

	w_offset get_base_offset();
};
