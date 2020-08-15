#pragma once

struct GE_API w_ui_mgr
{
	std::unique_ptr<w_ui_theme> theme = nullptr;

	bool mouse_is_visible = false;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int hot_id = -1;

	void init();
	void set_mouse_visible( bool visible );
	void draw_topmost();

	void im_reset();
	[[nodiscard]] e_im_result im_active( std::string_view label, w_rect rc, w_ui_style& ui_style );
	e_im_result im_passive( w_rect rc, w_ui_style& ui_style );

	[[nodiscard]] bool is_mouse_inside( w_rect& rc ) const;

	bool owning_layer_is_topmost = false;
	int im_automatic_id = 0;
};
