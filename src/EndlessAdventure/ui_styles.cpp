
#include "app_header.h"

// ----------------------------------------------------------------------------

w_ui_style_tile::w_ui_style_tile()
{
	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
}

void w_ui_style_tile::draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked )
{
	w_color bracket_color = w_color::black;
	bracket_color.a = 0.0f;

	if( being_clicked && being_hovered )
	{
		bracket_color = w_color::white;
		bracket_color.a = 1.0f;
	}
	else if( being_hovered )
	{
		bracket_color = w_color::white;
		bracket_color.a = 0.5f;
	}

	RENDER
		->begin();

	for( int rl = room_layer::geometry ; rl < room_layer::max ; ++rl )
	{
		a_subtexture* subtex_tile = GAME->get_tile( GAME->rooms[ GAME->current_room_idx ].tile_ids[ rl ][ master_tile_idx ] )->subtex;
		RENDER
			->push_depth_nudge()
			->draw( subtex_tile, rc );
	}
		
	RENDER->push_depth_nudge()
		->push_rgba( bracket_color )
		->draw( selector_bracket, rc );

	RENDER->end();
}

e_im_result w_ui_style_tile::update_im_state( int id, w_rect rc )
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

w_ui_style_browser_tile::w_ui_style_browser_tile()
{
	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
}

void w_ui_style_browser_tile::draw( std::string_view label, w_rect& rc, bool being_hovered, bool being_clicked )
{
	w_color bracket_color = w_color::black;
	bracket_color.a = 0.0f;

	if( being_clicked && being_hovered )
	{
		bracket_color = w_color::white;
		bracket_color.a = 1.0f;
	}
	else if( being_hovered )
	{
		bracket_color = w_color::white;
		bracket_color.a = 0.5f;
	}

	a_subtexture* subtex_tile = GAME->get_tile( master_tile_idx )->subtex;
	RENDER
		->begin()

		->push_depth_nudge()
		->push_rgba( background_color, 0.125f )
		->draw_filled_rectangle( rc )
		->pop_rgba()
		
		->push_depth_nudge()
		->draw( subtex_tile, rc )
		
		->push_depth_nudge()
		->push_rgba( bracket_color )
		->draw( selector_bracket, rc )

		->end();
}

