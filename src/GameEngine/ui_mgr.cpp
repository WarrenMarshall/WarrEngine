
#include "master_pch.h"
#include "master_header.h"

void w_ui_mgr::init()
{
	theme = std::make_unique<w_ui_theme>();
}

void w_ui_mgr::set_mouse_visible( bool visible )
{
	this->is_mouse_visible = visible;
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
}

e_im_result w_ui_mgr::im_active( w_rect rc, const w_ui_style& ui_style )
{
	im_automatic_id++;

	e_im_result result = ui_style.update_im_state( im_automatic_id, rc );
	ui_style.draw( rc, hover_id == im_automatic_id, hot_id == im_automatic_id );

	return result;
}

e_im_result w_ui_mgr::im_passive( w_rect rc, const w_ui_style& ui_style )
{
	ui_style.draw( rc, false, false );

	return im_result::none;
}
