
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
	return c2AABBtoPoint( rc.as_c2AABB(), engine->input->mouse_vwindow_pos.as_c2v() );
}

