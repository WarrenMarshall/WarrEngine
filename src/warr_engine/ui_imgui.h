#pragma once

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
	std::optional<w_rect> last_rc_win = std::nullopt;
	w_rect last_rc_client;
	w_imgui_control current_control = {};

	// the results from the last control processed
	w_imgui_result result = {};

	std::unordered_map<hash, w_imgui_control_data*> hash_to_control_data;

	w_imgui();

	void reset();

	w_imgui* do_panel( hash tag = hash_none );
	w_imgui* do_push_button( hash tag = hash_none );
	w_imgui* do_checkbox( hash tag = hash_none );
	w_imgui* do_divider( hash tag = hash_none );
	w_imgui* do_spacer( hash tag = hash_none );
	w_imgui* do_label( hash tag = hash_none );
	w_imgui* do_slider( hash tag = hash_none );
	w_imgui* do_edit_box( hash tag = hash_none );

	w_imgui* set_text( std::string text );
	w_imgui* set_text_align( e_align align );
	w_imgui* set_slice_def( const std::string& tag );
	w_imgui* set_pos( const w_pos& pos );
	w_imgui* set_pos( e_imgui_flow flow );
	w_imgui* set_size( const w_sz& sz );
	w_imgui* set_interval( const float interval );
	w_imgui* set_align( w_rect rc_client, e_align align );

	void compute_clientrect_from_rect();
	w_imgui_result* finalize( w_imgui_control_data* data = nullptr );
	w_imgui_control_data* get_control_data( hash tag );

private:
	void set_last_control( w_imgui_control control );

	virtual void update_im_state( int id, w_imgui_control& control, bool is_hovered, bool is_hot );
	void draw( w_imgui_control& control, bool is_hovered, bool is_hot );
	void draw_slice_def( const w_imgui_control& control, const w_rect& rc_win, bool is_hovered, bool is_hot );
	void draw_texture( const w_imgui_control& control, const w_rect& rc, const a_texture* texture, bool is_hovered, bool is_hot );
	void draw_text( const w_imgui_control& control, const w_rect& rc_client, const w_color& color, bool is_hovered, bool is_hot );
	_NODISCARD w_offset get_click_offset( bool is_hovered, bool is_hot );
	_NODISCARD w_color get_adjusted_color( const w_color& base_color, bool is_hovered, bool is_hot );

	int im_automatic_id = 0;
	w_tween caret_blink_tween;
};
