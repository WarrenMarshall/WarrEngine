#pragma once

struct w_ui_mgr
{
	a_cursor* mouse_cursor = nullptr;
	a_font* ui_font = nullptr;
	bool visible = false;

	void init( const std::string& mouse_cursor_name );
	void set_mouse_visible( bool visible );
	void draw_top_level();
};
