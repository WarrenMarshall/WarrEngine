#include "master_pch.h"
#include "master_header.h"

w_game_controller::w_game_controller( int idx )
	: idx( idx )
{
	timer_repeat = std::make_unique<w_timer>( 150 );
}

void w_game_controller::update_button_state( e_input_id input_id, int xinput_button_bit )
{
	bool last_state = engine->input->button_states_last_frame[ input_id ];

	bool current_state = ( xinput_state.Gamepad.wButtons & xinput_button_bit ) > 0;
	engine->input->button_states[ input_id ] = current_state;

	if( !last_state && current_state )
	{
		w_input_event evt;
		evt.event_id = event_id::input_pressed;
		evt.input_id = input_id;

		engine->input->event_queue.emplace_back( std::move( evt ) );

		is_being_used = true;
	}
	else if( last_state && !current_state )
	{
		w_input_event evt;
		evt.event_id = event_id::input_released;
		evt.input_id = input_id;

		engine->input->event_queue.emplace_back( std::move( evt ) );
	}
	else if( last_state && current_state )
	{
		timer_repeat->update();
		if( timer_repeat->get_elapsed_count() )
		{
			w_input_event evt;
			evt.event_id = event_id::input_held;
			evt.input_id = input_id;

			engine->input->event_queue.emplace_back( std::move( evt ) );
		}
	}
}

void w_game_controller::update()
{
	rumble_time_remaining_ms -= engine->time->FTS_step_value_ms;
	if( rumble_time_remaining_ms <= 0 )
	{
		rumble_time_remaining_ms = 0;

		XINPUT_VIBRATION rumbler;
		rumbler.wLeftMotorSpeed = 0;
		rumbler.wRightMotorSpeed = 0;
		XInputSetState( idx, &rumbler );
	}

	// refresh the xinput state for this controller

	ZeroMemory( &xinput_state, sizeof( XINPUT_STATE ) );
	XInputGetState( idx, &xinput_state );

	// update state information anfd send events

	update_button_state( input_id::controller_button_a, XINPUT_GAMEPAD_A );
	update_button_state( input_id::controller_button_b, XINPUT_GAMEPAD_B );
	update_button_state( input_id::controller_button_x, XINPUT_GAMEPAD_X );
	update_button_state( input_id::controller_button_y, XINPUT_GAMEPAD_Y );
	update_button_state( input_id::controller_button_dpad_left, XINPUT_GAMEPAD_DPAD_LEFT );
	update_button_state( input_id::controller_button_dpad_right, XINPUT_GAMEPAD_DPAD_RIGHT );
	update_button_state( input_id::controller_button_dpad_up, XINPUT_GAMEPAD_DPAD_UP );
	update_button_state( input_id::controller_button_dpad_down, XINPUT_GAMEPAD_DPAD_DOWN );
	update_button_state( input_id::controller_button_left_thumb, XINPUT_GAMEPAD_LEFT_THUMB );
	update_button_state( input_id::controller_button_right_thumb, XINPUT_GAMEPAD_RIGHT_THUMB );
	update_button_state( input_id::controller_button_left_shoulder, XINPUT_GAMEPAD_LEFT_SHOULDER );
	update_button_state( input_id::controller_button_right_shoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER );
}

void w_game_controller::play_rumble( e_rumble_effect effect )
{
	int rumble_max = 65535;
	int intensity = 65535;
	int duration_ms = 600;

	switch( effect )
	{
		case rumble_effect::medium:
		{
			intensity = static_cast<int>( rumble_max * 0.75f );
			duration_ms = 400;
		}
		break;

		case rumble_effect::small:
		{
			intensity = static_cast<int>( rumble_max * 0.5f );
			duration_ms = 300;
		}
		break;

		case rumble_effect::tiny:
		{
			intensity = static_cast<int>( rumble_max * 0.35f );
			duration_ms = 200;
		}
		break;

		case rumble_effect::none:
		{
			intensity = 0;
			duration_ms = 100;
		}
		break;
	}

	play_rumble( intensity, duration_ms );
}

void w_game_controller::play_rumble( int intensity, int ms )
{
	XINPUT_VIBRATION rumbler;
	rumbler.wLeftMotorSpeed = static_cast<WORD>( intensity );
	rumbler.wRightMotorSpeed = static_cast<WORD>( intensity );
	XInputSetState( idx, &rumbler );

	rumble_time_remaining_ms = static_cast<float>( ms );
}
