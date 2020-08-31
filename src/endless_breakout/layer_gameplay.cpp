
#include "app_header.h"

layer_gameplay::layer_gameplay()
{
}

void layer_gameplay::push()
{
	walls = spawn_entity<w_entity>();
	float wt = 16.0f;
	walls->add_component<ec_collider>()->init_as_box( w_vec2( 0.0f, 0.0f ), w_vec2( wt, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_vec2( v_window_w - wt, 0.0f ), w_vec2( v_window_w, v_window_h ) );
	walls->add_component<ec_collider>()->init_as_box( w_vec2( 0.0f, 0.0f ), w_vec2( v_window_w, wt ) );
	walls->add_component<ec_collider>()->init_as_box( w_vec2( 0.0f, v_window_h - wt ), w_vec2( v_window_w, v_window_h ) );

	player = spawn_entity<e_player>( { v_window_hw, v_window_h - 12 }, 0.0f, 0.75f );

	ball = spawn_entity<e_ball>( { v_window_hw, v_window_hh }, 0.0f, 0.75f );
	ball->add_component<ec_collider>()->init_as_circle( 12 );
	ball->forces.emplace_back( std::make_unique<w_force>( w_vec2::get_random_unit(), 200.0f ) );

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
