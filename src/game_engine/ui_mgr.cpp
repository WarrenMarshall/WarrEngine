
#include "master_pch.h"
#include "master_header.h"

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
	// #uitodo - optional support for circles here would be nice for round UI elements
	return c2AABBtoPoint( rc, engine->input->mouse_vwindow_pos );
}

void w_ui_mgr::im_reset()
{
	im_automatic_id = 0;
	owning_layer_is_topmost = false;
}

e_im_result w_ui_mgr::im_active( std::string_view label, w_rect rc, w_ui_style& ui_style )
{
	im_automatic_id++;

	e_im_result result = im_result::none;

	if( owning_layer_is_topmost )
	{
		result = ui_style.update_im_state( im_automatic_id, rc );
	}

	ui_style.draw( label, rc, hover_id == im_automatic_id, hot_id == im_automatic_id );

	return result;
}

e_im_result w_ui_mgr::im_passive( w_rect rc, w_ui_style& ui_style )
{
	ui_style.draw( "", rc, false, false );

	return im_result::none;
}
