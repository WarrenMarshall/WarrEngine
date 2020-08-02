
#include "master_pch.h"
#include "master_header.h"

e_im_result w_ui_style::update_im_state( int id, w_rect rc ) const
{
	e_im_result imresult = im_result::none;

	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	rc.w -= 1.0f;
	rc.h -= 1.0f;

	e_button_state bs_left = engine->input->get_button_state( input_id::mouse_button_left );
	bool mouse_is_inside = UI->is_mouse_inside( rc );

	if( mouse_is_inside )
	{
		if( bs_left == button_state::up || ( bs_left == button_state::held && UI->hot_id == id ) )
		{
			UI->hover_id = id;
		}
		else if( bs_left == button_state::pressed )
		{
			UI->hot_id = id;
		}
		else if( bs_left == button_state::released )
		{
			if( UI->hot_id == id && UI->hover_id == id )
			{
				imresult |= im_result::left_clicked;
			}
			UI->hover_id = UI->hot_id = -1;
		}
	}
	else
	{
		if( UI->hover_id == id )
		{
			UI->hover_id = -1;
		}

		if( bs_left == button_state::released && UI->hot_id == id )
		{
			UI->hot_id = -1;
		}
	}

	if( UI->hover_id == id )
	{
		imresult |= im_result::hovered;
	}
	if( UI->hot_id == id )
	{
		imresult |= im_result::hot;
	}

	return imresult;
}

// ----------------------------------------------------------------------------

w_ui_style_pushbutton::w_ui_style_pushbutton( a_9slice_def* slice_def, a_subtexture* subtex )
	: slice_def( slice_def ), subtex( subtex )
{
}

void w_ui_style_pushbutton::draw( w_rect& rc, bool being_hovered, bool being_clicked ) const
{
	w_color final_color = W_COLOR_DARK_GREY;
	w_vec2 rc_click_offset = { 0, 0 };
	if( being_clicked )
	{
		w_color::scale( final_color, 1.75f );
	}
	else if( being_hovered )
	{
		w_color::scale( final_color, 1.25f );
	}

	if( being_hovered && being_clicked )
	{
		rc_click_offset = { 1, 1 };
	}

	w_rect rc_draw = { rc.x + rc_click_offset.x, rc.y + rc_click_offset.y, rc.w, rc.h };

	RENDER->begin()
		->push_depth_nudge();

	if( slice_def )
	{
		RENDER->push_rgb( final_color )
			->draw_sliced( slice_def, rc_draw );
	}

	if( subtex )
	{
		RENDER->push_rgb( W_COLOR_WHITE )
			->push_depth_nudge()
			->draw_sprite( subtex, w_rect( rc_draw.x + ( rc.w / 2 ), rc_draw.y + ( rc.h / 2 ), ( rc.w / 2 ), ( rc.h / 2 ) ) );
	}

	RENDER->end();
}

// ----------------------------------------------------------------------------

w_ui_style_panel::w_ui_style_panel( a_9slice_def* slice_def )
	: slice_def( slice_def )
{
}

void w_ui_style_panel::draw( w_rect& rc, bool being_hovered, bool being_clicked ) const
{
	RENDER
		->begin()
		->push_depth_nudge()
		->push_rgb( W_COLOR_DARK_GREY )
		->draw_sliced( slice_def, rc )
		->end();
}

// ----------------------------------------------------------------------------

w_ui_style_tile::w_ui_style_tile( a_subtexture* tile_subtex )
	: tile_subtex( tile_subtex )
{
	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
}

void w_ui_style_tile::draw( w_rect& rc, bool being_hovered, bool being_clicked ) const
{
	w_color bracket_color = W_COLOR_BLACK;
	bracket_color.a = 0.0f;

	if( being_clicked && being_hovered )
	{
		bracket_color = W_COLOR_WHITE;
		bracket_color.a = 1.0f;
	}
	else if( being_hovered )
	{
		bracket_color = W_COLOR_WHITE;
		bracket_color.a = 0.5f;
	}

	RENDER
		->begin()
		->push_depth_nudge();

	RENDER
		->draw( tile_subtex, rc );

	RENDER
		->push_rgba( bracket_color )
		->push_depth_nudge()
		->draw( selector_bracket, rc );

	RENDER->end();
}

e_im_result w_ui_style_tile::update_im_state( int id, w_rect rc ) const
{
	e_im_result imresult = im_result::none;

	/*
		 reduce the size of the hit rectangle. this gives more breathing room
		 when mousing over tightly packed UI elements and stops the user
		 from being able to highlight/click more than one at a time.
	*/

	rc.w -= 1.0f;
	rc.h -= 1.0f;

	e_button_state bs_left = engine->input->get_button_state( input_id::mouse_button_left );
	bool mouse_is_inside = UI->is_mouse_inside( rc );

	if( mouse_is_inside )
	{
		if( bs_left == button_state::up || ( bs_left == button_state::held && UI->hot_id != -1 ) )
		{
			UI->hover_id = id;
		}
		else if( bs_left == button_state::pressed )
		{
			UI->hot_id = id;
		}
		else if( bs_left == button_state::released )
		{
			if( UI->hot_id == id && UI->hover_id == id )
			{
				imresult |= im_result::left_clicked;
			}
			UI->hover_id = UI->hot_id = -1;
		}
	}
	else
	{
		if( bs_left == button_state::released && UI->hot_id == id )
		{
			UI->hot_id = -1;
		}
	}

	if( UI->hover_id == id )
	{
		imresult |= im_result::hovered;
	}
	if( UI->hot_id == id )
	{
		imresult |= im_result::hot;
	}

	return imresult;
}

// ----------------------------------------------------------------------------

void w_ui_theme::init()
{
	mouse_cursor = engine->get_asset<a_cursor>( "ui_cursor", b_silent( true ) );
	small_font = engine->get_asset<a_font>( "ui_simple_font" );

	panel_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
	button_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
}

void w_ui_theme::draw_topmost()
{
	// mouse cursor

	if( engine->ui->mouse_is_visible && mouse_cursor != nullptr )
	{
		RENDER
			->begin()
			->push_depth( zdepth_engine )
			->draw( mouse_cursor->subtex,
					w_rect(
						engine->input->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
						engine->input->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y
					)
			)
			->end();
	}
}
