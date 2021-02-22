
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
			scoped_render_state;

			render_state.z = zdepth_topmost;

			auto viewport_pos = w_coord::window_to_viewport_pos( engine->input->mouse_window_pos );
			auto ui_pos = w_coord::viewport_to_ui_pos( viewport_pos );

			w_render::draw_quad( mouse_cursor->texture,
				w_rect(
				( ui_pos.x - mouse_cursor->hotspot_offset.x ),
				( ui_pos.y - mouse_cursor->hotspot_offset.y )
			)
			);
		}
	}
}

bool w_ui_mgr::is_mouse_inside( const w_rect& rc ) const
{
	// note : because we apply the ui_scale, this is only usable for UI based rectangles
	w_rect rc_scaled = rc * ui_scale;

	// note : all ui interactions are reduced to checking the mouse position inside of an AABB
	auto viewport_pos = w_coord::window_to_viewport_pos( engine->input->mouse_window_pos );
	return c2AABBtoPoint( rc_scaled.as_c2AABB(), viewport_pos.as_c2v() );
}

void w_ui_mgr::show_msg_box( const std::string& msg )
{
	msg_box.msg = msg;
	LAYER_MGR->push<layer_msg_box>();
}
