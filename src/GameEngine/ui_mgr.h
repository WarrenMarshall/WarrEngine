#pragma once

struct w_ui_mgr
{
	std::unique_ptr<w_ui_theme> theme = nullptr;

	bool is_mouse_visible = false;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int clicked_id = -1;

	void init();
	void set_mouse_visible( bool visible );
	void draw_topmost();

	bool im_button( int id, const w_rect& rc );
	bool im_image_button( int id, const w_rect& rc, a_subtexture* subtexture );

	bool is_mouse_inside( w_rect& rc ) const;

private:
	bool update_im_state( int id, w_rect rc );
};
