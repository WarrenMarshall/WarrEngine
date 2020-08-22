#pragma once

// ----------------------------------------------------------------------------

struct w_ui_style
{
	w_ui_style() = default;

	w_color color_slice_def = w_color::dark_grey;
	w_color color_label = w_color::light_grey;
	w_offset clicked_offset = w_offset( 1, 1 );

	virtual e_im_result update_im_state( int id, w_rect rc );
	virtual void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) = 0;
	w_offset get_click_offset( bool being_hovered, bool being_clicked );
	w_color get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked );
};

// ----------------------------------------------------------------------------

struct w_ui_style_button : w_ui_style
{
	w_ui_style_button();

	// [optional] background for the button
	a_9slice_def* slice_def = nullptr;

	// [optional] image to draw on the button
	a_subtexture* subtex = nullptr;
	w_offset subtex_pos_offset = w_offset( -1, -1 );
	w_sz subtex_sz = w_sz( -1, -1 );

	void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

// ----------------------------------------------------------------------------

struct w_ui_style_panel : w_ui_style
{
	a_9slice_def* slice_def = nullptr;

	w_ui_style_panel();

	void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

// ----------------------------------------------------------------------------

struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;
	a_font* large_font = nullptr;

	a_9slice_def* panel_slice_def = nullptr;
	a_9slice_def* button_slice_def = nullptr;
	float control_padding = 4.0f;

	void init();
	void draw_topmost();
};
