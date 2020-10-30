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
	std::string label;
	a_9slice_def* slice_def = nullptr;
	a_subtexture* subtexture = nullptr;
	w_rect rc = { 0,0,0,0 };	// full rectangle
	w_rect crc = { 0,0,0,0 };	// client rectangle
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

	// parent control stacka and accessors
	std::vector<w_imgui_control> parent_stack = {};
	void push_parent();
	void pop_parent();
	std::optional<w_imgui_control*> get_parent_control();

	void reset();

	w_imgui* init_push_button();
	w_imgui* init_panel();

	w_imgui* set_label( const std::string& label );
	w_imgui* set_slice_def( a_9slice_def* slice_def );
	w_imgui* set_subtexture( a_subtexture* subtexture );

	w_imgui* set_rect( w_rect rc );
	w_imgui* set_rect( e_imgui_flow flow );
	w_imgui* set_rect( e_imgui_flow flow, w_sz sz );

	void calc_client_rect();
	w_imgui_result* go();

private:
	void active();
	void passive();

	_NODISCARD virtual e_im_result update_im_state( int id, w_rect rc );
	void draw( const w_imgui_control& control, bool being_hovered, bool being_clicked );
	_NODISCARD w_offset get_click_offset( bool being_hovered, bool being_clicked );
	_NODISCARD w_color get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked );

	int im_automatic_id = 0;
};
