#pragma once

struct w_ui_mgr
{
	w_imgui imgui;
	a_cursor* mouse_cursor = nullptr;

	void init();
	void draw_topmost();
	bool is_mouse_inside( w_rect& rc ) const;
};
