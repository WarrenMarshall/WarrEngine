#pragma once

// ----------------------------------------------------------------------------

struct w_ui_style_attrib
{
	w_color color = w_color::white;
	e_align alignment = align::invalid;
	std::optional<w_vec2> pos = std::nullopt;
	std::optional<w_vec2> sz = std::nullopt;
};

// ----------------------------------------------------------------------------

struct w_ui_style
{
	w_ui_style() = default;
	virtual ~w_ui_style() = default;

	// base
	w_ui_style_attrib base_attrib;

	// label
	w_ui_style_attrib label_attrib;

	// clicking
	w_offset clicked_offset = w_offset( 1, 1 );

	// body
	std::optional<a_9slice_def*> slice_def;

	// image
	w_ui_style_attrib subtex_attrib;

	_NODISCARD virtual e_im_result get_im_state( int id, w_rect rc );
	virtual void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked ) = 0;
	_NODISCARD w_offset get_click_offset( bool being_hovered, bool being_clicked );
	_NODISCARD w_color get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked );
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

	a_9slice_def* sd_panel = nullptr;
	a_9slice_def* sd_push_button = nullptr;
	a_subtexture* st_checkbox_clear = nullptr;
	a_subtexture* st_checkbox_checked = nullptr;

	float control_padding = 4.0f;

	std::unique_ptr<w_ui_style_button> default_button_style = nullptr;

	void init();
	void draw_topmost();
};
