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

struct w_imgui
{
	bool owning_layer_is_topmost = false;

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

	void set_origin( const w_vec2& origin );
	void clear_origin();

	void reset();

	w_imgui_result push_button( std::string_view label, w_rect rc );
	w_imgui_result panel( w_rect rc );

private:
	w_imgui_result active( std::string_view label, w_rect rc, w_ui_style& ui_style );
	w_imgui_result passive( std::string_view label, w_rect rc, w_ui_style& ui_style );

	int im_automatic_id = 0;
};
