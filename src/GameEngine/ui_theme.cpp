
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_ui_style_pushbutton::w_ui_style_pushbutton()
{
	slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
}

void w_ui_style_pushbutton::draw( w_rect& rc, bool being_hovered, bool being_clicked )
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

	RENDER
		->begin()
		->push_color( final_color )
		->push_depth( 100 )
		->draw_sliced( slice_def, w_rect( rc.x + rc_click_offset.x, rc.y + rc_click_offset.y, rc.w, rc.h ) )
		->end();
}

// ----------------------------------------------------------------------------

w_ui_style_panel::w_ui_style_panel()
{
	slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
}

void w_ui_style_panel::draw( w_rect& rc, bool being_hovered, bool being_clicked )
{
	RENDER
		->begin()
		->push_color( W_COLOR_DARK_GREY )
		->push_depth( 100 )
		->draw_sliced( slice_def, rc )
		->end();
}

// ----------------------------------------------------------------------------

w_ui_style_tile::w_ui_style_tile( a_subtexture* tile_subtexture )
	: tile_subtexture( tile_subtexture )
{
	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
}

void w_ui_style_tile::draw( w_rect& rc, bool being_hovered, bool being_clicked )
{
	w_color bracket_color = W_COLOR_BLACK;
	bracket_color.a = 0.0f;

	if( being_clicked )
	{
		bracket_color = W_COLOR_WHITE;
		bracket_color.a = 1.0f;
	}
	else if( being_hovered )
	{
		bracket_color = W_COLOR_WHITE;
		bracket_color.a = 0.5f;
	}

	RENDER->begin();

	RENDER
		->push_depth( 100 )
		->draw( tile_subtexture, rc );

	RENDER
		->push_color( bracket_color )
		->push_alpha( bracket_color.a )
		->push_depth_nudge( 10 )
		->draw( selector_bracket, rc );

	RENDER->end();
}

// ----------------------------------------------------------------------------

void w_ui_theme::init()
{
	mouse_cursor = engine->get_asset<a_cursor>( "ui_cursor", b_silent( true ) );
	small_font = engine->get_asset<a_font>( "ui_simple_font" );
}

void w_ui_theme::draw_topmost()
{
	// mouse cursor

	if( engine->ui->is_mouse_visible && mouse_cursor != nullptr )
	{
		RENDER
			->begin()
			->push_depth( 1000.0f )
			->draw( mouse_cursor->img,
					w_rect(
						engine->input->mouse_vwindow_pos.x - mouse_cursor->hotspot_offset.x,
						engine->input->mouse_vwindow_pos.y - mouse_cursor->hotspot_offset.y
					)
			)
			->end();
	}
}
