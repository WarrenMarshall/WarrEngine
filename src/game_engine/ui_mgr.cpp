
#include "master_pch.h"
#include "master_header.h"

void w_ui_mgr::init()
{
	mouse_cursor = a_cursor::find( "mouse_cursor_default", b_silent( true ) );
}

void w_ui_mgr::draw_topmost()
{
	// mouse cursor

	if( mouse_cursor && engine->window->mouse_mode == mouse_mode::custom )
	{
		RENDER
			->begin()
			->push_depth( zdepth_topmost )
			->draw( mouse_cursor->subtex,
					w_rect(
						engine->input->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
						engine->input->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y
					)
			)
			->end();
	}
}

bool w_ui_mgr::is_mouse_inside( w_rect& rc ) const
{
	return c2AABBtoPoint( rc.as_c2AABB(), engine->input->mouse_vwindow_pos.as_c2v() );
}
