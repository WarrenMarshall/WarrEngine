#pragma once

#define UI_PADDING 2.0f

// ----------------------------------------------------------------------------

struct w_imgui_result
{
	e_im_result result = im_result::none;

	void operator=( e_im_result res );

	_NODISCARD bool was_left_clicked();
	_NODISCARD bool was_right_clicked();
};

// ----------------------------------------------------------------------------

struct w_imgui_control
{
	bool is_active = true;
	const char* tag = nullptr;
	std::string label;
	e_align label_align = align::centered;
	a_9slice_def* slice_def = nullptr;
	a_subtexture* subtexture[ 2 ];
	e_align subtexture_align = align::fill;
	w_rect rc = { 0,0,0,0 };	// full rectangle
	w_rect crc = { 0,0,0,0 };	// client rectangle

	std::function<int(const char*)> cb_get_subtexture_idx;
};

// ----------------------------------------------------------------------------

struct w_imgui
{
	bool containing_layer_is_topmost = false;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int hot_id = -1;

	// the current control being set up or drawn
	w_pos flow_right, flow_down;
	std::optional<w_imgui_control> last_control = std::nullopt;
	w_imgui_control control = {};

	// the results from the last control processed
	w_imgui_result result = {};

	void reset();

	// this holds COPIES of any controls that are tagged when drawn.
	//
	// tagging controls lets you find them later that frame if needed.
	std::unordered_map<const char*, w_imgui_control> tagged_controls;
	w_imgui* set_last_control_from_tag( const char* tag );
	w_imgui_control find_control( const char* tag );
	w_imgui* clear_last_control();

	w_imgui* init_push_button( const char* tag = nullptr );
	w_imgui* init_checkbox( const char* tag = nullptr );
	w_imgui* init_panel( const char* tag = nullptr );

	w_imgui* set_label( const std::string& label, e_align align = align::centered );
	w_imgui* set_slice_def( a_9slice_def* slice_def );
	w_imgui* set_subtexture( a_subtexture* subtexture, e_align align = align::fill, int idx = -1 );
	w_imgui* set_callback( std::function<int( const char* )> cb_func );

	w_imgui* set_rect( w_rect rc );
	w_imgui* set_rect( e_imgui_flow flow );
	w_imgui* set_rect( e_imgui_flow flow, w_sz sz );

	void calc_client_rect();
	w_imgui_result* finalize();

private:
	void _active();
	void _passive();
	void _set_as_last_control( w_imgui_control control );

	_NODISCARD virtual e_im_result _update_im_state( int id, w_rect rc );
	void _draw( w_imgui_control& control, bool being_hovered, bool being_clicked );
	_NODISCARD w_offset _get_click_offset( bool being_hovered, bool being_clicked );
	_NODISCARD w_color _get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked );

	int im_automatic_id = 0;
};
