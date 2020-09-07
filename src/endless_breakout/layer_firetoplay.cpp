
#include "app_header.h"

void layer_firetoplay::push()
{
	w_layer::push();

	scale_tween = std::make_unique<w_tween>( tween_type::half_sine, 0.0f, 360.0f, 180.0f );
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
		->push_scale( 1.5f + ( scale_tween->get_fval() * 0.5f ) )
		->draw_string( engine->pixel_font, "PRESS ANYTHING TO PLAY", w_rect( v_window_hw, v_window_hh ) )
		->end();
}

void layer_firetoplay::update()
{
	w_layer::update();

	scale_tween->update();
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
