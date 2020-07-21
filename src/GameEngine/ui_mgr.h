#pragma once

struct w_ui_mgr
{
	a_cursor* mouse_cursor = nullptr;
	a_font* ui_font = nullptr;
	bool visible = false;

	// which UI control the mouse is hovering over
	e_ui_id hover_id = e_ui_id::invalid;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	e_ui_id clicked_id = e_ui_id::invalid;

	void init( const std::string& mouse_cursor_name );
	void set_mouse_visible( bool visible );
	void draw_top_level();

	bool im_button( e_ui_id id, const a_9slice_def* slice_def, const w_color& color, w_rect& rc );
	bool im_image( e_ui_id id, const a_subtexture* subtexture, const w_color& color, w_rect& rc );

	bool is_mouse_inside( w_rect& rc ) const;
	bool update_im_state( e_ui_id id, w_rect rc );
};
