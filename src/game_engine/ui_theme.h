#pragma once

// ----------------------------------------------------------------------------

#if 0
struct w_ui_style_attrib
{
	w_color color = w_color::white;
	e_align alignment = align::invalid;
	std::optional<w_vec2> pos = std::nullopt;
	std::optional<w_vec2> sz = std::nullopt;
};
#endif

// ----------------------------------------------------------------------------

struct w_ui_style
{
	w_ui_style() = default;
	~w_ui_style() = default;

	//std::function<void( void )> cb_was_left_clicked;
	a_9slice_def* slice_def = nullptr;
	a_subtexture* subtexture = nullptr;

	_NODISCARD virtual e_im_result get_im_state( int id, w_rect rc );
	void draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked );
	_NODISCARD w_offset get_click_offset( bool being_hovered, bool being_clicked );
	_NODISCARD w_color get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked );
};

// ----------------------------------------------------------------------------

#if 0
struct w_ui_theme
{
	a_cursor* mouse_cursor = nullptr;

	a_9slice_def* sd_panel = nullptr;
	a_9slice_def* sd_push_button = nullptr;
	a_subtexture* st_checkbox_clear = nullptr;
	a_subtexture* st_checkbox_checked = nullptr;

	float control_padding = 4.0f;

	//std::unique_ptr<w_ui_style_button> default_button_style = nullptr;

	void init();
	void draw_topmost();
};
#endif
