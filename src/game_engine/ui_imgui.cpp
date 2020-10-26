
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

void w_imgui_result::operator=( e_im_result res )
{
	result = res;
}

bool w_imgui_result::was_left_clicked()
{
	return ( result == im_result::left_clicked );
}

bool w_imgui_result::was_right_clicked()
{
	return ( result == im_result::right_clicked );
}

void w_imgui::set_origin( const w_vec2& origin )
{
	this->origin = origin;
}

void w_imgui::clear_origin()
{
	origin = w_vec2::zero;
}

// ----------------------------------------------------------------------------

void w_imgui::reset()
{
	im_automatic_id = 0;
	owning_layer_is_topmost = false;
}

w_imgui_result w_imgui::push_button( std::string_view label, w_rect rc, w_ui_style_button* style )
{
	w_ui_style_button default_style;

	if( !style )
	{
		style = &default_style;
	}

	return active( label, rc, style );
}

w_imgui_result w_imgui::panel( w_rect rc, w_ui_style_panel* style )
{
	w_ui_style_panel default_style;

	if( !style )
	{
		style = &default_style;
	}

	return passive( "", rc, style );
}

w_imgui_result w_imgui::active( std::string_view label, w_rect rc, w_ui_style* ui_style )
{
	last_rect = rc;

	im_automatic_id++;

	rc.x += origin.x;
	rc.y += origin.y;

	w_imgui_result result;

	if( owning_layer_is_topmost )
	{
		result = ui_style->get_im_state( im_automatic_id, rc );
	}

	ui_style->draw( label, rc, hover_id == im_automatic_id, hot_id == im_automatic_id );

	return result;
}

w_imgui_result w_imgui::passive( std::string_view label, w_rect rc, w_ui_style* ui_style )
{
	last_rect = rc;

	rc.x += origin.x;
	rc.y += origin.y;

	ui_style->draw( label, rc, false, false );

	return w_imgui_result();
}
