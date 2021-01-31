#pragma once

// ----------------------------------------------------------------------------

struct w_imgui_result
{
	e_im_result code = im_result::none;

	// the mouse position, normalized to within the client
	// rect of the control.
	w_pos click_pos = {};
	w_vec2 click_pct = {};

	void operator=( const e_im_result res );

	[[nodiscard]] bool was_left_clicked();
};

// ----------------------------------------------------------------------------

struct w_imgui
{
	w_imgui_callback default_callback;

	// #ui - this feels clunky. there has to be a better way to express this situation/condition.
	bool containing_layer_is_topmost = false;
	w_layer* current_layer = nullptr;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int hot_id = -1;

	// the current control being set up or drawn
	w_pos flow_right_pos, flow_down_pos;
	w_rect last_rc_win, last_rc_client;
	w_imgui_control current_control = {};

	// the results from the last control processed
	w_imgui_result result = {};

	w_imgui();

	void reset();

	w_imgui* do_panel( hash tag = 0 );
	w_imgui* do_push_button( hash tag = 0 );
	w_imgui* do_checkbox( hash tag = 0 );
	w_imgui* do_divider( hash tag = 0 );
	w_imgui* do_label( hash tag = 0 );
	w_imgui* do_slider( hash tag = 0 );
	w_imgui* do_edit_box( hash tag = 0 );

	w_imgui* set_text( const std::string& text );
	w_imgui* set_text_align( e_align align );
	w_imgui* set_slice_def( const std::string& tag );
	w_imgui* set_position( const w_pos& pos );
	w_imgui* set_position( e_imgui_flow flow );
	w_imgui* set_size( const w_sz& sz );
	w_imgui* set_interval( const float interval );

	void compute_clientrect_from_rect();
	w_imgui_result* finalize();

private:
	void set_last_control( w_imgui_control control );

	virtual void update_im_state( int id, const w_imgui_control& control, bool is_hovered, bool is_hot );
	void draw( w_imgui_control& control, bool is_hovered, bool is_hot );
	void draw_slice_def( const w_imgui_control& control, const w_rect& rc_win, bool is_hovered, bool is_hot );
	void draw_texture( const w_imgui_control& control, const w_rect& rc, const a_texture* texture, bool is_hovered, bool is_hot );
	void draw_text( const w_imgui_control& control, const w_rect& rc_client, const w_color& color, bool is_hovered, bool is_hot );
	[[nodiscard]] w_offset get_click_offset( bool is_hovered, bool is_hot );
	[[nodiscard]] w_color get_adjusted_color( const w_color& base_color, bool is_hovered, bool is_hot );

	int im_automatic_id = 0;
	w_tween caret_blink_tween;
};
