#pragma once

struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;
	a_font* small_font = nullptr;
	a_subtexture* selector_bracket = nullptr;
	a_9slice_def* button_slice_def = nullptr;
	a_9slice_def* panel_slice_def = nullptr;

	void init();
	void draw_topmost();

	void draw_panel( int id, const w_rect& rc );
	void draw_button( int id, const w_rect& rc );
	void draw_image_button( int id, const w_rect& rc, const a_subtexture* subtexture );
};
