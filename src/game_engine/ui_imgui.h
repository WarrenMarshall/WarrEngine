#pragma once

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
	w_ui_style style;
	w_rect rc = { 0,0,0,0 };

	w_imgui_result result = {};
};

struct w_imgui
{
	bool containing_layer_is_topmost = false;

	// which UI control the mouse is hovering over
	int hover_id = -1;

	// a hot id is one the user has clicked the mouse on but hasn't unclicked yet
	int hot_id = -1;

	// a temporary origin to start drawing new controls relative to. useful for
	// easily positioning controls inside of a panel or other parent control.
	w_vec2 origin = w_vec2::zero;

	// saved information about the last control that was drawn. can be useful
	// for positioning other controls on the screen.
	w_rect last_rect = { 0,0,0,0 };

	w_imgui_control control;

	void set_origin( const w_vec2& origin );
	void clear_origin();

	void reset();

	w_imgui* begin_push_button();
	w_imgui* begin_panel();

	w_imgui* set_label( const std::string& label );
	w_imgui* set_slice_def( a_9slice_def* slice_def );
	w_imgui* set_subtexture( a_subtexture* subtexture );
	w_imgui* set_rect( w_rect rc );

	w_imgui_result* end();

private:
	w_imgui_result* active();
	w_imgui_result* passive();

	int im_automatic_id = 0;
};
