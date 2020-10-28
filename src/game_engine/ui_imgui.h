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
	a_9slice_def* slice_def = nullptr;
	a_subtexture* subtexture = nullptr;
	w_rect rc = { 0,0,0,0 };

	w_imgui_result result = {};
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
	w_imgui_control control;

	// an optional control that can be used to easily position other controls
	std::optional<w_imgui_control> origin_control = std::nullopt;

	void clear_origin();
	void reset();

	w_imgui* begin_push_button();
	w_imgui* begin_panel();

	w_imgui* set_label( const std::string& label );
	w_imgui* set_slice_def( a_9slice_def* slice_def );
	w_imgui* set_subtexture( a_subtexture* subtexture );
	w_imgui* set_rect( w_rect rc );
	w_imgui* set_last_as_origin();

	w_imgui_result* end();

private:
	w_imgui_result* active();
	w_imgui_result* passive();

	_NODISCARD virtual e_im_result update_im_state( int id, w_rect rc );
	void draw( const w_imgui_control& control, bool being_hovered, bool being_clicked );
	_NODISCARD w_offset get_click_offset( bool being_hovered, bool being_clicked );
	_NODISCARD w_color get_adjusted_color( w_color base_color, bool being_hovered, bool being_clicked );

	int im_automatic_id = 0;
};
