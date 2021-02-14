
#include "master_pch.h"
#include "master_header.h"

void w_ui_mgr::init()
{
	imgui = std::make_unique<w_imgui>();
	mouse_cursor = a_cursor::find( "mouse_cursor_default", b_silent( true ) );
}

void w_ui_mgr::draw_topmost()
{
	// mouse cursor

	if( mouse_cursor && engine->window->mouse_mode == mouse_mode::custom )
	{
		{
			scoped_render_push_pop;

			RENDER->set_z_depth( zdepth_topmost );
			render_state.scale = ui_window_scale;
			w_render::draw( mouse_cursor->texture,
				w_rect(
				( engine->input->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x ),
				( engine->input->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y )
			)
			);
		}
	}
}

bool w_ui_mgr::is_mouse_inside( const w_rect& rc ) const
{
	// note : because we apply the ui_window_scale, this is only usable for UI based rectangles
	w_rect rc_scaled = rc * ui_window_scale;

	// note : all ui interactions are reduced to checking the mouse position inside of an AABB
	return c2AABBtoPoint( rc_scaled.as_c2AABB(), engine->input->mouse_vwindow_pos.as_c2v() );
}

void w_ui_mgr::show_msg_box( const std::string& msg )
{
	msg_box.msg = msg;
	LAYER_MGR->push<layer_msg_box>();
}
