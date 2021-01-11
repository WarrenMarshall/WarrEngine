#pragma once

#define UI_PADDING 2.0f

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

	w_imgui* init_panel( hash tag = 0 );
	w_imgui* init_push_button( hash tag = 0 );
	w_imgui* init_checkbox( hash tag = 0 );

	w_imgui* set_label( const std::string& label );
	w_imgui* set_label_align( e_align align );
	w_imgui* set_slice_def( a_9slice_def* slice_def );
	w_imgui* set_texture( e_imgui_control_state state, a_texture* texture );
	w_imgui* set_texture_align( e_align align );
	w_imgui* set_position( const w_vec2& pos );
	w_imgui* set_position( e_imgui_flow flow );
	w_imgui* set_size( const w_sz& size );

	void compute_clientrect_from_rect();
	w_imgui_result* finalize();

private:
	void _active();
	void _passive();
	void _set_as_last_control( w_imgui_control control );

	[[nodiscard]] virtual e_im_result _update_im_state( int id, const w_rect& rc );
	void _draw( w_imgui_control& control, bool being_hovered, bool being_clicked );
	[[nodiscard]] w_offset _get_click_offset( bool being_hovered, bool being_clicked );
	[[nodiscard]] w_color _get_adjusted_color( const w_color& base_color, bool being_hovered, bool being_clicked );

	int im_automatic_id = 0;
};
