
#pragma once

struct w_imgui_result
{
	e_im_result code = im_result::none;

	// the mouse position, normalized to within the client
	// rect of the control.
	w_pos click_pos = {};
	w_vec2 click_pct = {};

	void operator=( const e_im_result res );
};
