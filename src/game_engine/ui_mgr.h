#pragma once

struct w_im_result
{
	e_im_result result = im_result::none;

	void operator=( e_im_result res );

	_NODISCARD bool was_left_clicked();
	_NODISCARD bool was_right_clicked();
};

// ----------------------------------------------------------------------------

struct w_ui_mgr
{
	std::unique_ptr<w_ui_theme> theme = nullptr;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int hot_id = -1;

	void init();
	void draw_topmost();

	void im_reset();

	_NODISCARD w_im_result im_push_button( std::string_view label, w_rect rc );

	_NODISCARD w_im_result im_active( std::string_view label, w_rect rc, w_ui_style& ui_style );
	w_im_result im_passive( w_rect rc, w_ui_style& ui_style );

	_NODISCARD bool is_mouse_inside( w_rect& rc ) const;

	bool owning_layer_is_topmost = false;
	int im_automatic_id = 0;
};
