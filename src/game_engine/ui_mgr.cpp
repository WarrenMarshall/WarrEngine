
#include "master_pch.h"
#include "master_header.h"

void w_im_result::operator=( e_im_result res )
{
	result = res;
}

bool w_im_result::was_left_clicked()
{
	return ( result == im_result::left_clicked );
}

bool w_im_result::was_right_clicked()
{
	return ( result == im_result::right_clicked );
}

// ----------------------------------------------------------------------------

void w_ui_mgr::init()
{
	theme = std::make_unique<w_ui_theme>();
}

void w_ui_mgr::draw_topmost()
{
	theme->draw_topmost();
}

bool w_ui_mgr::is_mouse_inside( w_rect& rc ) const
{
	return c2AABBtoPoint( rc, engine->input->mouse_vwindow_pos );
}

void w_ui_mgr::im_reset()
{
	im_automatic_id = 0;
	owning_layer_is_topmost = false;
}

w_im_result w_ui_mgr::im_push_button( std::string_view label, w_rect rc )
{
	w_ui_style_button style;
	style.base_attrib.color = w_color::dark_grey;
	style.label_attrib.color = w_color::red;

	return im_active( label, rc, style );
}

w_im_result w_ui_mgr::im_active( std::string_view label, w_rect rc, w_ui_style& ui_style )
{
	im_automatic_id++;

	w_im_result result;

	if( owning_layer_is_topmost )
	{
		result = ui_style.get_im_state( im_automatic_id, rc );
	}

	ui_style.draw( label, rc, hover_id == im_automatic_id, hot_id == im_automatic_id );

	return result;
}

w_im_result w_ui_mgr::im_passive( w_rect rc, w_ui_style& ui_style )
{
	ui_style.draw( "", rc, false, false );

	return w_im_result();
}
