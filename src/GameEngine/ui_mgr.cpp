
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

bool w_ui_mgr::update_im_state( e_ui_id id, w_rect rc )
{
	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	rc.w -= 0.5f;
	rc.h -= 0.5f;

	bool result = false;
	e_button_state bs = engine->input_mgr->get_button_state( e_input_id::mouse_button_left );

	if( is_mouse_inside( rc ) && clicked_id == e_ui_id::invalid )
	{
		hover_id = id;
	}

	if( bs == e_button_state::pressed )
	{
		if( is_mouse_inside( rc ) )
		{
			clicked_id = id;
		}
	}
	else if( bs == e_button_state::released )
	{
		if( is_mouse_inside( rc ) && clicked_id == id )
		{
			result = true;
		}

		hover_id = clicked_id = e_ui_id::invalid;
	}

	return result;
}

bool w_ui_mgr::im_button( e_ui_id id, const a_9slice_def* slice_def, const w_color& color, w_rect& rc )
{
	bool result = update_im_state( id, rc );

	// ----------------------------------------------------------------------------

	w_color final_color = color;
	if( hover_id == id )
	{
		w_color::scale( final_color, 1.25f );
	}
	if( clicked_id == id )
	{
		w_color::scale( final_color, 1.75f );
	}

	// ----------------------------------------------------------------------------

	RENDER
		->begin()
		->push_color( final_color )
		->push_depth( 100 )
		->draw_sliced( slice_def, rc )
		->end();
	
	return result;
}

bool w_ui_mgr::im_image( e_ui_id id, const a_subtexture* subtexture, const w_color& color, w_rect& rc )
{
	bool result = update_im_state( id, rc );

	// ----------------------------------------------------------------------------

	w_color final_color = color;
	if( hover_id == id )
	{
		w_color::scale( final_color, 1.25f );
	}
	if( clicked_id == id )
	{
		w_color::scale( final_color, 1.75f );
	}

	// ----------------------------------------------------------------------------

	RENDER
		->begin()
		->push_color( final_color )
		->push_depth( 100 )
		->draw( subtexture, rc )
		->end();

	return result;
}
