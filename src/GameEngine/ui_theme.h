#pragma once

struct w_ui_style
{
	w_ui_style() = default;

	virtual void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const = 0;
	virtual e_im_result update_im_state( int id, w_rect rc ) const;
};

struct w_ui_style_pushbutton : w_ui_style
{
	w_ui_style_pushbutton( a_9slice_def* slice_def, a_subtexture* subtex );

	a_9slice_def* slice_def = nullptr;
	a_subtexture* subtex = nullptr;

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const override;
};

struct w_ui_style_panel : w_ui_style
{
	a_9slice_def* slice_def = nullptr;

	w_ui_style_panel( a_9slice_def* slice_def );

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const override;
};

struct w_ui_style_tile : w_ui_style
{
	a_subtexture* tile_subtex = nullptr;
	a_subtexture* selector_bracket = nullptr;

	w_ui_style_tile() = delete;
	w_ui_style_tile( a_subtexture* tile_subtex );

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const override;
	e_im_result update_im_state( int id, w_rect rc ) const override;
};

// ----------------------------------------------------------------------------

struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;
	a_font* small_font = nullptr;

	a_9slice_def* panel_slice_def = nullptr;
	a_9slice_def* button_slice_def = nullptr;

	void init();
	void draw_topmost();
};
