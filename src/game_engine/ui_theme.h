#pragma once

// ----------------------------------------------------------------------------

struct w_ui_style_attrib
{
	w_color color = w_color::white;
	std::optional<w_vec2> pos;
	std::optional<w_vec2> sz;
};

// ----------------------------------------------------------------------------

struct w_ui_style
{
	w_ui_style() = default;

	// base
	w_ui_style_attrib base_attrib;

	// label
	w_ui_style_attrib label_attrib;

	// clicking
	w_offset clicked_offset = w_offset( 1, 1 );

	// body
	std::optional<a_9slice_def*> slice_def;

	// image
	std::optional<a_subtexture*> subtex;
	w_ui_style_attrib subtex_attrib;

	virtual e_im_result update_im_state( int id, w_rect rc );
	virtual void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) = 0;
	w_offset get_click_offset( bool being_hovered, bool being_clicked );
	w_color get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked );
};

// ----------------------------------------------------------------------------

struct w_ui_style_button : w_ui_style
{
	w_ui_style_button();

	virtual void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

// ----------------------------------------------------------------------------

struct w_ui_style_panel : w_ui_style
{
	// #uitodo	- expand this to have "inactive / hover / clicked" slice def options
	//			- if hover or clicked are nullopt, then use the inactive for everything
	a_9slice_def* slice_def = nullptr;

	w_ui_style_panel();

	virtual void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) override;
};

// ----------------------------------------------------------------------------

struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;

	a_9slice_def* panel_slice_def = nullptr;
	a_9slice_def* button_up_slice_def = nullptr;
	a_9slice_def* button_down_slice_def = nullptr;
	float control_padding = 4.0f;

	void init();
	void draw_topmost();
};
