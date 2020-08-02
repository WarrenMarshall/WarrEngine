#pragma once

struct w_ui_mgr
{
	std::unique_ptr<w_ui_theme> theme = nullptr;

	bool is_mouse_visible = false;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int hot_id = -1;

	void init();
	void set_mouse_visible( bool visible );
	void draw_topmost();

	void im_reset();
	e_im_result im_active( const w_layer* layer, w_rect rc, const w_ui_style& ui_style );
	e_im_result im_passive( const w_layer* layer, w_rect rc, const w_ui_style& ui_style );

	bool is_mouse_inside( w_rect& rc ) const;

	int im_automatic_id = 0;
};
