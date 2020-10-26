
#include "app_header.h"

void layer_edit_list::push()
{
	game->load_data();
}

void layer_edit_list::draw()
{
	w_layer::draw();

	w_ui_style_button bitmap_button_style;
	bitmap_button_style.slice_def = nullptr;
	bitmap_button_style.subtex_list[ 0 ] = a_subtexture::find( "gear_icon" );
	//bitmap_button_style.subtex_list[ 1 ] = a_subtexture::find( "ui_box_checkmark" );

	/*
	if( IMGUI->push_button( ))

	if( UI->im_active( "", w_rect( v_window_w - 24, 6, 24, 24) , *( style_clear_button.get() ) ) & im_result::left_clicked )
	{
		for( auto& artist : GAME->artists )
		{
			artist.approved = false;
		}
		GAME->save_data();
	}
	*/

	/*
	float ypos = 6.0f;

	for( auto& artist : GAME->artists )
	{
		RENDER
			->begin()
			->push_depth_nudge( 10 );

		w_ui_style* style = artist.approved ? style_approved_button.get() : style_waiting_button.get();
		if( UI->im_active( artist.name, w_rect( 8, ypos, 130, 16 ), *( style ) ) & im_result::left_clicked )
		{
			artist.approved = !artist.approved;
			GAME->save_data();
		}
		RENDER->end();

		ypos += 18.0f;
	}
	*/
}
