#pragma once

struct w_imgui_control_data
{
	std::variant<bool, float, std::string> data;
	e_imgui_control_type control_type = imgui_control_type::none;
	int max_length = -1;
	std::string valid_char_list;
	size_t caret_pos = 0;

	w_imgui_control_data( bool value );
	w_imgui_control_data( std::string value );
	w_imgui_control_data( float value );

	void operator=( bool value );
	void operator=( std::string value );
	void operator=( float value );
};
