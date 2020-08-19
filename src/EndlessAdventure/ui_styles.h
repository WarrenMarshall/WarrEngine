#pragma once

// ----------------------------------------------------------------------------
// tiles for the main editing window

struct w_ui_style_tile : w_ui_style
{
	int master_tile_idx = -1;
	a_subtexture* selector_bracket = nullptr;

	w_ui_style_tile();

	virtual e_im_result update_im_state( int id, w_rect rc ) final;
	virtual void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

// ----------------------------------------------------------------------------
// tiles for the tile browser

struct w_ui_style_browser_tile final : w_ui_style_tile
{
	w_color background_color = w_color::black;

	w_ui_style_browser_tile();

	virtual void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) final;
};

// ----------------------------------------------------------------------------
