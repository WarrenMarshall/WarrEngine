
#include "app_header.h"

layer_background::layer_background()
{
	draws_completely_solid = true;
}

void layer_background::push()
{
	background_gradient = engine->get_asset<a_gradient>( "background_gradient" );
	click_snd = engine->get_asset<a_sound>( "ui_click_01" );
	snd2 = engine->get_asset<a_sound>( "spaceball" );
	music = engine->get_asset<a_music>( "background_music" );

	spawn_entity<e_background_stars>( { v_window_hw, v_window_hh } );

	music->play();
}

void layer_background::draw()
{
	w_layer::draw();
}

bool layer_background::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_released )
	{
		switch( evt->input_id )
		{
			case input_id::controller_button_a:
				click_snd->play();
				return true;

			case input_id::controller_button_b:
				snd2->play();
				return true;
		}
	}

	return false;
}
