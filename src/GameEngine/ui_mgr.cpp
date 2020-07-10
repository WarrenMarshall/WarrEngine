
#include "master_pch.h"
#include "master_header.h"

void w_ui_mgr::init( const std::string& mouse_cursor_name )
{
	mouse_cursor = engine->get_asset<a_cursor>( mouse_cursor_name, b_silent(true) );
	ui_font = engine->get_asset<a_font>( "ui_simple_font" );
}

void w_ui_mgr::set_mouse_visible( bool visible )
{
	this->visible = visible;
}

void w_ui_mgr::draw_top_level()
{
	// mouse cursor

	if( visible && mouse_cursor != nullptr )
	{
		MATRIX
			->push_identity()
			->translate( w_vec2(
				-v_window_hw + engine->input_mgr->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
				v_window_hh - engine->input_mgr->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y )
			);

		RENDER
			->begin()
			->push_depth( 1000.0f )
			->draw( mouse_cursor->img )
			->end();

		MATRIX->pop();
	}
}
