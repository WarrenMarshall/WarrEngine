
#include "master_pch.h"
#include "master_header.h"

void w_ui_theme::init()
{
	mouse_cursor = a_cursor::find( "mouse_cursor_default", b_silent( true ) );
}

// called at the end of each frame, this is for drawing anything that
// needs to appear on top of everything else. like the mouse cursor.

void w_ui_theme::draw_topmost()
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
