#pragma once

struct w_ui_mgr
{
	std::unique_ptr<w_imgui> imgui = nullptr;
	a_cursor* mouse_cursor = nullptr;

	void init();
	void draw_topmost();
	bool is_mouse_inside( const w_rect& rc ) const;

	// if a control is meant to retain focus across frames, store the tag here.
	hash tag_focus = hash_none;

	// message box

	void show_msg_box( const std::string& msg );

	struct
	{
		std::string msg;
	} msg_box;
};
