
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Game_Controller::Game_Controller( int32_t player_id )
	: player_id( player_id )
{
	timer_repeat = Timer( 150 );
}

void Game_Controller::update_button_state( e_input_id input_id, int32_t xinput_button_bit )
{
	bool last_state = g_engine->input_mgr.button_states_last_frame[ (int32_t)input_id ];

	bool current_state = ( xinput_state.Gamepad.wButtons & xinput_button_bit ) > 0;
	g_engine->input_mgr.button_states[ (int32_t)input_id ] = current_state;

	if( !last_state and current_state )
	{
		Input_Event evt;
		evt.event_id = e_event_id::input_pressed;
		evt.input_id = input_id;

		g_engine->input_mgr.event_queue.push_back( evt );

		is_being_used = true;
	}
	else if( last_state and !current_state )
	{
		Input_Event evt;
		evt.event_id = e_event_id::input_released;
		evt.input_id = input_id;

		g_engine->input_mgr.event_queue.push_back( evt );
	}
	else if( last_state and current_state )
	{
		if( timer_repeat.get_elapsed() )
		{
			Input_Event evt;
			evt.event_id = e_event_id::input_held;
			evt.input_id = input_id;

			g_engine->input_mgr.event_queue.push_back( evt );
		}
	}
}

// updates the internal state of the controller, from xinput

void Game_Controller::update_state()
{
	// refresh the xinput state for this controller

	ZeroMemory( &xinput_state, sizeof( XINPUT_STATE ) );
	XInputGetState( player_id, &xinput_state );
}

void Game_Controller::update()
{
	rumble_time_remaining_ms -= fixed_time_step::ms_per_step;
	if( rumble_time_remaining_ms <= 0 )
	{
		rumble_time_remaining_ms = 0;

		XINPUT_VIBRATION rumbler {};
		rumbler.wLeftMotorSpeed = 0;
		rumbler.wRightMotorSpeed = 0;
		XInputSetState( player_id, &rumbler );
	}

	update_state();
}

void Game_Controller::play_rumble( e_rumble_effect effect )
{
	int32_t rumble_max = 65535;
	int32_t intensity = 65535;
	int32_t duration_ms = 600;

	switch( effect )
	{
		case e_rumble_effect::medium:
		{
			intensity = (int32_t)( rumble_max * 0.75f );
			duration_ms = 400;
			break;
		}

		case e_rumble_effect::small:
		{
			intensity = (int32_t)( rumble_max * 0.5f );
			duration_ms = 300;
			break;
		}

		case e_rumble_effect::tiny:
		{
			intensity = (int32_t)( rumble_max * 0.35f );
			duration_ms = 200;
			break;
		}

		case e_rumble_effect::none:
		{
			intensity = 0;
			duration_ms = 100;
			break;
		}
	}

	play_rumble( intensity, duration_ms );
}

void Game_Controller::play_rumble( int32_t intensity, int32_t ms )
{
	XINPUT_VIBRATION rumbler {};
	rumbler.wLeftMotorSpeed = (WORD)( intensity );
	rumbler.wRightMotorSpeed = (WORD)( intensity );
	XInputSetState( player_id, &rumbler );

	rumble_time_remaining_ms = (float_t)( ms );
}

}
