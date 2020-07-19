#include "master_pch.h"
#include "master_header.h"

w_game_controller::w_game_controller( int idx )
	: idx( idx )
{
	timer_repeat = std::make_unique<w_timer>( 150 );
}

void w_game_controller::update_button_state( e_input_id input_id, int xinput_button_bit )
{
	bool last_state = engine->input_mgr->button_states[static_cast<int>( input_id )];
	bool current_state = (xinput_state.Gamepad.wButtons & xinput_button_bit) > 0;
	engine->input_mgr->button_states[static_cast<int>( input_id )] = current_state;

	if( !last_state && current_state )
	{
		w_input_event evt;
		evt.event_id = e_event_id::input_pressed;
		evt.input_id = input_id;

		engine->input_mgr->event_queue.emplace_back( std::move( evt ) );

		is_being_used = true;
		timer_repeat->reset();
	}
	else if( last_state && !current_state )
	{
		w_input_event evt;
		evt.event_id = e_event_id::input_released;
		evt.input_id = input_id;

		engine->input_mgr->event_queue.emplace_back( std::move( evt ) );
	}
	else if( last_state && current_state )
	{
		timer_repeat->update();

		if( timer_repeat->get_elapsed_count() )
		{
			w_input_event evt;
			evt.event_id = e_event_id::input_pressed;
			evt.input_id = input_id;

			engine->input_mgr->event_queue.emplace_back( std::move( evt ) );
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

	update_button_state( e_input_id::controller_button_a, XINPUT_GAMEPAD_A );
	update_button_state( e_input_id::controller_button_b, XINPUT_GAMEPAD_B );
	update_button_state( e_input_id::controller_button_x, XINPUT_GAMEPAD_X );
	update_button_state( e_input_id::controller_button_y, XINPUT_GAMEPAD_Y );
	update_button_state( e_input_id::controller_button_dpad_left, XINPUT_GAMEPAD_DPAD_LEFT );
	update_button_state( e_input_id::controller_button_dpad_right, XINPUT_GAMEPAD_DPAD_RIGHT );
	update_button_state( e_input_id::controller_button_dpad_up, XINPUT_GAMEPAD_DPAD_UP );
	update_button_state( e_input_id::controller_button_dpad_down, XINPUT_GAMEPAD_DPAD_DOWN );
	update_button_state( e_input_id::controller_button_left_thumb, XINPUT_GAMEPAD_LEFT_THUMB );
	update_button_state( e_input_id::controller_button_right_thumb, XINPUT_GAMEPAD_RIGHT_THUMB );
	update_button_state( e_input_id::controller_button_left_shoulder, XINPUT_GAMEPAD_LEFT_SHOULDER );
	update_button_state( e_input_id::controller_button_right_shoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER );
}

void w_game_controller::play_rumble( int intensity, int ms )
{
	XINPUT_VIBRATION rumbler;
	rumbler.wLeftMotorSpeed = intensity;
	rumbler.wRightMotorSpeed = intensity;
	XInputSetState( idx, &rumbler );

	rumble_time_remaining_ms = static_cast<float>( ms );
}
