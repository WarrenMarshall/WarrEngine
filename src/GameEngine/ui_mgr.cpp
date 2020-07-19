
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
		RENDER
			->begin()
			->push_depth( 1000.0f )
			->draw_line( { 0,0 }, engine->input_mgr->mouse_vwindow_pos )
			->draw( mouse_cursor->img,
					w_rect(
						engine->input_mgr->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
						engine->input_mgr->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y
					)
			)
			->end();
	}
}

bool w_ui_mgr::is_mouse_inside( w_rect& rc ) const
{
	return c2AABBtoPoint( rc, engine->input_mgr->mouse_vwindow_pos );
}

bool w_ui_mgr::im_button( e_ui_id id, const a_9slice_def* slice_def, w_rect& rc )
{
	bool result = false;

	bool button_is_down = engine->input_mgr->is_button_down( e_input_id::mouse_button_left );

	if( button_is_down )
	{
		if( is_mouse_inside( rc ) )
		{
			if( hover_id == id )
			{
				clicked_id = id;
			}
		}
	}
	else
	{
		if( is_mouse_inside( rc ) )
		{
			if( clicked_id == id && hover_id == id )
			{
				result = true;
				clicked_id = e_ui_id::invalid;
			}

			hover_id = id;
		}
		else
		{
			hover_id = clicked_id = e_ui_id::invalid;
		}
	}

	// ----------------------------------------------------------------------------

	w_color color = W_COLOR_DARK_GREY;
	if( hover_id == id )
	{
		color = W_COLOR_GREY;
	}
	if( clicked_id == id )
	{
		color = W_COLOR_LIGHT_GREY;
	}

	RENDER
		->begin()
		->push_color( color )
		->push_depth( 100 )
		->draw_sliced( slice_def, rc )
		->end();
	
	return result;
}

bool w_ui_mgr::im_image( e_ui_id id, const a_subtexture* subtexture, w_rect& rc )
{
	bool result = false;

	bool button_is_down = engine->input_mgr->is_button_down( e_input_id::mouse_button_left );

	if( button_is_down )
	{
		if( is_mouse_inside( rc ) )
		{
			if( hover_id == id )
			{
				clicked_id = id;
			}
		}
	}
	else
	{
		if( is_mouse_inside( rc ) )
		{
			if( clicked_id == id && hover_id == id )
			{
				result = true;
				clicked_id = e_ui_id::invalid;
			}

			hover_id = id;
		}
		else
		{
			hover_id = clicked_id = e_ui_id::invalid;
		}
	}

	w_color color = RENDER->rs_color_stack.top();
	if( hover_id == id )
	{
		color = W_COLOR_GREY;
	}
	if( clicked_id == id )
	{
		color = W_COLOR_WHITE;
	}

	RENDER
		->begin()
		->push_color( color )
		->push_depth( 100 )
		->draw( subtexture, rc )
		->end();

	return result;
}
