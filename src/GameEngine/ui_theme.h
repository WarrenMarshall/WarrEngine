#pragma once

struct w_ui_style
{
	w_ui_style() = default;
	w_ui_style( const std::string_view label );

	std::string_view label;

	virtual e_im_result update_im_state( int id, w_rect rc ) const;
	virtual void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const = 0;
};

struct w_ui_style_pushbutton : w_ui_style
{
	w_ui_style_pushbutton() = default;
	w_ui_style_pushbutton( const std::string_view label, a_9slice_def* slice_def, a_subtexture* subtex );

	a_9slice_def* slice_def = nullptr;
	a_subtexture* subtex = nullptr;
	w_sz subtex_sz = w_sz( -1, -1 );

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const override;
};

struct w_ui_style_radiobutton : w_ui_style_pushbutton
{
	w_ui_style_radiobutton( const std::string_view label, a_9slice_def* slice_def, a_subtexture* subtex, a_subtexture* subtex_radio_on );

	a_subtexture* subtex_radio_on = nullptr;

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const override;
};

struct w_ui_style_panel : w_ui_style
{
	a_9slice_def* slice_def = nullptr;

	w_ui_style_panel( const std::string_view label, a_9slice_def* slice_def );

	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const override;
};

struct w_ui_style_tile : w_ui_style
{
	a_subtexture* tile_subtex = nullptr;
	a_subtexture* selector_bracket = nullptr;

	w_ui_style_tile() = delete;
	w_ui_style_tile( const std::string_view label, a_subtexture* tile_subtex );

	e_im_result update_im_state( int id, w_rect rc ) const override;
	void draw( w_rect& rc, bool being_hovered, bool being_clicked ) const override;
};

// ----------------------------------------------------------------------------

struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;
	a_font* small_font = nullptr;
	a_font* large_font = nullptr;

	a_9slice_def* panel_slice_def = nullptr;
	a_9slice_def* button_slice_def = nullptr;

	void init();
	void draw_topmost();
};
