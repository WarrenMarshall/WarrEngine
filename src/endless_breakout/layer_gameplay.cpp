
#include "app_header.h"

layer_gameplay::layer_gameplay()
{
	draws_completely_solid = false;
}

void layer_gameplay::push()
{
	player = spawn_entity<e_player>( { v_window_hw, v_window_h } );
	ball = spawn_entity<e_ball>( { v_window_hw, v_window_hh } );
	ball->forces.emplace_back( std::make_unique<w_force>( w_vec2::normalize( w_vec2( w_random::getf(), w_random::getf() ) ), 350.0f ) );

	engine->window->set_mouse_mode( mouse_mode::locked );
}

bool layer_gameplay::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::mouse )
		{
			player->pos.x += evt->mouse.delta.x;
			player->pos.x = w_clamp( player->pos.x, 0.0f, v_window_w );
			return true;
		}
	}
	return false;
}
