
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

bool w_ui_mgr::update_im_state( int id, w_rect rc )
{
	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	rc.w -= 0.5f;
	rc.h -= 0.5f;

	bool result = false;
	e_button_state bs = engine->input->get_button_state( e_input_id::mouse_button_left );
	bool mouse_is_inside = is_mouse_inside( rc );

	if( mouse_is_inside )
	{
		if( bs == e_button_state::up || ( bs == e_button_state::held && clicked_id == id ) )
		{
			hover_id = id;
		}
		else if( bs == e_button_state::pressed )
		{
			clicked_id = id;
		}
		else if( bs == e_button_state::released )
		{
			if( clicked_id == id && hover_id == id )
			{
				result = true;
			}
			hover_id = clicked_id = -1;
		}
	}
	else
	{
		if( hover_id == id )
		{
			hover_id = -1;
		}

		if( bs == e_button_state::released && clicked_id == id )
		{
			clicked_id = -1;
		}
	}

	return result;
}

bool w_ui_mgr::im_button( int id, const w_rect& rc )
{
	bool result = update_im_state( id, rc );
	theme->draw_button( id, rc );

	return result;
}

bool w_ui_mgr::im_image_button( int id, const w_rect& rc, a_subtexture* subtexture )
{
	bool result = update_im_state( id, rc );
	theme->draw_image_button( id, rc, subtexture );

	return result;
}
