#pragma once

// ----------------------------------------------------------------------------
// tiles for the main editing window

struct w_ui_style_tile final : w_ui_style
{
	int idx = -1;
	//a_subtexture* subtex_tile = nullptr;
	a_subtexture* selector_bracket = nullptr;

	w_ui_style_tile();

	e_im_result update_im_state( int id, w_rect rc ) override;
	void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) final;
};

// ----------------------------------------------------------------------------
// tiles for the tile browser

struct w_ui_style_browser_tile final : w_ui_style
{
	a_subtexture* subtex_tile = nullptr;
	a_subtexture* selector_bracket = nullptr;
	w_color background_color = w_color::red;

	w_ui_style_browser_tile();

	e_im_result update_im_state( int id, w_rect rc ) override;
	void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) final;
};

// ----------------------------------------------------------------------------
