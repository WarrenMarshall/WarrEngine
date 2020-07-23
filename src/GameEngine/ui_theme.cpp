#include "master_pch.h"
#include "master_header.h"

void w_ui_theme::init()
{
	mouse_cursor = engine->get_asset<a_cursor>( "ui_cursor", b_silent( true ) );
	small_font = engine->get_asset<a_font>( "ui_simple_font" );
	selector_bracket = engine->get_asset<a_subtexture>( "selector_bracket" );
	button_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
	panel_slice_def = engine->get_asset<a_9slice_def>( "ui_simple_panel" );
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

void w_ui_theme::draw_panel( int id, const w_rect& rc )
{
	RENDER->begin()
		->push_depth( 50.0f )
		->push_color( W_COLOR_DARK_GREY )
		->draw_sliced( panel_slice_def, rc )
		->end();
}

void w_ui_theme::draw_button( int id, const w_rect& rc )
{
	// ----------------------------------------------------------------------------

	w_color final_color = W_COLOR_DARK_GREY;
	if( UI->hover_id == id )
	{
		w_color::scale( final_color, 1.25f );
	}
	if( UI->clicked_id == id )
	{
		w_color::scale( final_color, 1.75f );
	}

	// ----------------------------------------------------------------------------

	RENDER
		->begin()
		->push_color( final_color )
		->push_depth( 100 )
		->draw_sliced( button_slice_def, rc )
		->end();
}

void w_ui_theme::draw_image_button( int id, const w_rect& rc, const a_subtexture* subtexture )
{
	w_color final_color = W_COLOR_DARK_GREY;
	if( UI->hover_id == id )
	{
		w_color::scale( final_color, 1.25f );
	}
	if( UI->clicked_id == id )
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
}
