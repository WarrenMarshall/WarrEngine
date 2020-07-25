#pragma once

struct w_ui_style
{
	w_ui_style() = default;

	virtual void draw( w_rect& rc, bool being_hovered, bool being_clicked ) = 0;
};

struct w_ui_style_pushbutton : w_ui_style
{
	w_ui_style_pushbutton();

	a_9slice_def* slice_def = nullptr;

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

struct w_ui_style_panel : w_ui_style
{
	a_9slice_def* slice_def = nullptr;

	w_ui_style_panel();

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

struct w_ui_style_tile : w_ui_style
{
	a_subtexture* tile_subtexture = nullptr;
	a_subtexture* selector_bracket = nullptr;

	w_ui_style_tile() = delete;
	w_ui_style_tile( a_subtexture* tile_subtexture );

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

// ----------------------------------------------------------------------------

struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;
	a_font* small_font = nullptr;

	void init();
	void draw_topmost();
};
