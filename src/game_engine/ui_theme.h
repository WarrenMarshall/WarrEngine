#pragma once

struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;

	void init();
	void draw_topmost();
};
