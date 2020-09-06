
#include "app_header.h"

void layer_firetoplay::push()
{
	w_layer::push();
}

void layer_firetoplay::pop()
{
	w_layer::pop();
}

void layer_firetoplay::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_align( align::centered )
		->push_rgba( w_color( 1.0f, 0.75f, 0.5f, w_random::getf_range( 0.5, 1.0f ) ) )
		->push_scale( 2.0f )
		->draw_string( engine->pixel_font, "PRESS ANYTHING TO PLAY", w_rect( v_window_hw, v_window_hh ) )
		->end();
}

bool layer_firetoplay::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_released )
	{
		GAME->new_game();
		engine->layer_mgr->pop();
	}

	return true;
}
