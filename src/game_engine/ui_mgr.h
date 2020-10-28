#pragma once

struct w_ui_mgr
{
	std::unique_ptr<w_ui_theme> theme = nullptr;

	w_imgui imgui;

	void init();
	void draw_topmost();
	bool is_mouse_inside( w_rect& rc ) const;
};
