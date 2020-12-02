#include "master_pch.h"
#include "master_header.h"

w_gamepad::w_gamepad( int player_id )
	: player_id( player_id )
{
	timer_repeat = std::make_unique<w_timer>( 150 );
}

void w_gamepad::update_button_state( e_input_id input_id, int xinput_button_bit )
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

// updates the internal state of the controller, from xinput

void w_gamepad::update_state()
{
	// refresh the xinput state for this controller

	ZeroMemory( &xinput_state, sizeof( XINPUT_STATE ) );
	XInputGetState( player_id, &xinput_state );
}

void w_gamepad::update()
{
	rumble_time_remaining_ms -= engine->time->FTS_step_value_ms;
	if( rumble_time_remaining_ms <= 0 )
	{
		rumble_time_remaining_ms = 0;

		XINPUT_VIBRATION rumbler;
		rumbler.wLeftMotorSpeed = 0;
		rumbler.wRightMotorSpeed = 0;
		XInputSetState( player_id, &rumbler );
	}

	update_state();
}

void w_gamepad::play_rumble( e_rumble_effect effect )
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

void w_gamepad::play_rumble( int intensity, int ms )
{
	XINPUT_VIBRATION rumbler;
	rumbler.wLeftMotorSpeed = static_cast<WORD>( intensity );
	rumbler.wRightMotorSpeed = static_cast<WORD>( intensity );
	XInputSetState( player_id, &rumbler );

	rumble_time_remaining_ms = static_cast<float>( ms );
}