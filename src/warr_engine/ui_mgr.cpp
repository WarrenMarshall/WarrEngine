
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
			->push_scale( ui_canvas_scale )
			->draw( mouse_cursor->texture,
					w_rect(
						(engine->input->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x),
						(engine->input->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y)
					)
			)
			->end();
	}
}

// note : because we apply the canvas scale, this is only usable for UI canvas based rectangles
bool w_ui_mgr::is_mouse_inside( const w_rect& rc ) const
{
	w_rect rc_scaled = rc * ui_canvas_scale;
	return c2AABBtoPoint( rc_scaled.as_c2AABB(), engine->input->mouse_vwindow_pos.as_c2v() );
}
