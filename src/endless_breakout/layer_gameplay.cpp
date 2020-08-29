
#include "app_header.h"

layer_gameplay::layer_gameplay()
{
	draws_completely_solid = false;
}

void layer_gameplay::push()
{
	player = spawn_entity<e_player>( { v_window_hw, v_window_h } );
	spawn_entity<e_ball>( { v_window_hw, v_window_hh } );

	engine->window->set_mouse_mode( mouse_mode::locked );
}

bool layer_gameplay::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_motion )
	{
		if( evt->input_id == input_id::mouse )
		{
			//log_msg( fmt::format( "{}, {}", evt->mouse.delta.x, evt->mouse.delta.y ) );
			player->pos.x += evt->mouse.delta.x;
			player->pos.x = w_clamp( player->pos.x, 0.0f, v_window_w );
			return true;
		}
	}
	return false;
}

void layer_gameplay::draw()
{
	w_layer::draw();

	engine->window->set_mouse_mode( mouse_mode::locked );
}
