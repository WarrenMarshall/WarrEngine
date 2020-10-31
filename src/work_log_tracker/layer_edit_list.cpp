
#include "app_header.h"

void layer_edit_list::push()
{
	game->load_data();
}

void layer_edit_list::draw()
{
	w_layer::draw();

	if( IMGUI->init_push_button()
		->set_subtexture( a_subtexture::find( "gear_icon" ) )
		->set_rect( { v_window_w - 24, 6, 24, 24 } )
		->finalize()
		->was_left_clicked() )
	{
		for( auto& artist : game->artists )
		{
			artist.approved = false;
		}
		game->save_data();
	}

	float ypos = 3.0f;

	for( auto& artist : game->artists )
	{
		RENDER
			->begin()
			->push_depth_nudge( 10 );

		if( !artist.approved )
		{
			RENDER->push_alpha( 0.25f );
		}

		IMGUI->clear_last_control();

		if(
			IMGUI->init_push_button()
			->set_label( artist.name )
			->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
			->set_rect( { 8, ypos, 130, 18 } )
			->finalize()
			->was_left_clicked() )
		{
			artist.approved = !artist.approved;
			game->save_data();
		}

		RENDER->end();

		ypos += 18.0f;
	}
}
