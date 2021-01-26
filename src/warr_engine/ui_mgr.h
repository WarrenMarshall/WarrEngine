#pragma once

struct w_ui_mgr
{
	std::unique_ptr<w_imgui> imgui = nullptr;
	a_cursor* mouse_cursor = nullptr;

	void init();
	void draw_topmost();
	bool is_mouse_inside( const w_rect& rc ) const;
};
