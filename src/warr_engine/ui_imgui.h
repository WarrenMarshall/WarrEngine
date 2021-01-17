#pragma once

// ----------------------------------------------------------------------------

struct w_imgui_result
{
	e_im_result result = im_result::none;

	void operator=( const e_im_result res );

	[[nodiscard]] bool was_left_clicked();
	[[nodiscard]] bool was_right_clicked();
};

// ----------------------------------------------------------------------------

struct w_imgui
{
	w_imgui_callback default_callback;
	w_imgui_callback* current_callback = nullptr;

	// #ui - this feels clunky. there has to be a better way to express this situation/condition.
	bool containing_layer_is_topmost = false;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int hot_id = -1;

	// the current control being set up or drawn
	w_pos flow_right, flow_down;
	std::optional<w_imgui_control> last_control = std::nullopt;
	w_imgui_control current_control = {};

	// the results from the last control processed
	w_imgui_result result = {};

	void reset();

	// this holds COPIES of any controls that are tagged when drawn.
	//
	// tagging controls lets you find them later that frame if needed.
	std::unordered_map<hash, w_imgui_control> tagged_controls;
	w_imgui* set_last_control_from_tag( hash tag );
	w_imgui_control find_control( hash tag );
	w_imgui* clear_last_control();

	void set_current_callback_from_current_layer();
	w_imgui* init_panel( hash tag = 0 );
	w_imgui* init_push_button( hash tag = 0 );
	w_imgui* init_checkbox( hash tag = 0 );
	w_imgui* init_divider( hash tag = 0 );
	w_imgui* init_label( hash tag = 0 );
	w_imgui* init_slider( hash tag = 0 );

	w_imgui* set_text( const std::string& text );
	w_imgui* set_text_align( e_align align );
	w_imgui* set_slice_def( const std::string& tag );
	w_imgui* set_position( const w_pos& pos );
	w_imgui* set_position( e_imgui_flow flow );
	w_imgui* set_size( const w_sz& sz );

	void compute_clientrect_from_rect();
	w_imgui_result* finalize();

private:
	void _active();
	void _passive();
	void _set_as_last_control( w_imgui_control control );

	[[nodiscard]] virtual e_im_result _update_im_state( int id, const w_rect& rc_win );
	void _draw( const w_imgui_control& control, bool being_hovered, bool being_clicked );
	void _draw_slice_def( const w_imgui_control& control, const w_rect& rc_win, bool being_hovered, bool being_clicked );
	void _draw_texture( const w_imgui_control& control, const w_rect& rc, const a_texture* texture, bool being_hovered, bool being_clicked );
	void _draw_text( const w_imgui_control& control, const w_rect& rc_client, bool being_hovered, bool being_clicked );
	[[nodiscard]] w_offset _get_click_offset( bool being_hovered, bool being_clicked );
	[[nodiscard]] w_color _get_adjusted_color( const w_color& base_color, bool being_hovered, bool being_clicked );

	int im_automatic_id = 0;
};
