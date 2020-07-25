
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

e_im_result w_ui_mgr::update_im_state( int id, w_rect rc )
{
	e_im_result imresult = e_im_result::none;

	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	rc.w -= 1.0f;
	rc.h -= 1.0f;

	bool result = false;
	e_button_state bs = engine->input->get_button_state( e_input_id::mouse_button_left );
	bool mouse_is_inside = is_mouse_inside( rc );

	if( mouse_is_inside )
	{
		if( bs == e_button_state::up || ( bs == e_button_state::held && hot_id == id ) )
		{
			hover_id = id;
		}
		else if( bs == e_button_state::pressed )
		{
			hot_id = id;
		}
		else if( bs == e_button_state::released )
		{
			if( hot_id == id && hover_id == id )
			{
				imresult |= e_im_result::clicked;
			}
			hover_id = hot_id = -1;
		}
	}
	else
	{
		if( hover_id == id )
		{
			hover_id = -1;
		}

		if( bs == e_button_state::released && hot_id == id )
		{
			hot_id = -1;
		}
	}

	if( hover_id == id )
	{
		imresult |= e_im_result::hovered;
	}
	if( hot_id == id )
	{
		imresult |= e_im_result::hot;
	}

	return imresult;
}

void w_ui_mgr::im_reset()
{
	im_id = 0;
}

e_im_result w_ui_mgr::im_active( w_rect rc, w_ui_style* ui_style )
{
	im_id++;

	e_im_result result = update_im_state( im_id, rc );
	ui_style->draw( rc, hover_id == im_id, hot_id == im_id );

	return result;
}

e_im_result w_ui_mgr::im_passive( w_rect rc, w_ui_style* ui_style )
{
	ui_style->draw( rc, false, false );

	return e_im_result::none;
}
