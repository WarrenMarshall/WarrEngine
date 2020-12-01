
#include "master_pch.h"
#include "master_header.h"

/*
	input system design notes

	- buttons being pressed and released are sent as listener events
		- (keypresses, mouse buttons, or controller buttons)

	- buttons can also be queried as needed
		- (keypresses, mouse buttons, or controller buttons)

	- mouse motion
		- only sent as an event, can't be queried
		- the delta is updated in real time via the callback function but the
		  event is only sent once per call to Update()

	- controller motion
		- must be queried, it's never sent as an event
		- controller motion is pressure sensitive so you need to poll each frame
		  to see how hard the user is pushing the stick or the trigger you want
*/

static w_vec2 last_mouse_pos( 0, 0 );

void mouse_motion_callback( GLFWwindow* window, double xpos, double ypos )
{
	engine->input->mouse_move_delta.x += static_cast<float>( xpos ) - last_mouse_pos.x;
	engine->input->mouse_move_delta.y += static_cast<float>( ypos ) - last_mouse_pos.y;

	last_mouse_pos = w_vec2( static_cast<float>( xpos ), static_cast<float>( ypos ) );

	// convert the window space window mouse position into a position on the virtual screen.
	auto ratio = ( v_window_w / engine->window->viewport_pos_sz.w );
	auto vx = ( xpos - engine->window->viewport_pos_sz.x ) * ratio;
	auto vy = ( ypos - engine->window->viewport_pos_sz.y ) * ratio;

	// only update the position if the mouse is moving over the virtual window itself
	if( vx >= 0 && vx <= v_window_w && vy >= 0 && vy <= v_window_h )
	{
		// integer mouse coordinates make things like UI click
		// detection simpler and less error prone
		engine->input->mouse_vwindow_pos.x = round( static_cast<float>( vx ) );
		engine->input->mouse_vwindow_pos.y = round( static_cast<float>( vy ) );
	}
}

void joystick_callback( int jid, int event )
{
	if( event == GLFW_CONNECTED )
	{
		//log( "The joystick was connected" );
	}
	else if( event == GLFW_DISCONNECTED )
	{
		//log( "The joystick was disconnected" );
	}

	engine->input->refresh_gamepad();
}

// ----------------------------------------------------------------------------

void w_input::init()
{
	// minimize memory reallocations by over reserving the queue
	event_queue.reserve( event_queue_max_size );

	refresh_gamepad();

	// set up callback so we know when controllers are connected/disconnected
	glfwSetJoystickCallback( joystick_callback );

	// callbacks so we can collect user input
	glfwSetCursorPosCallback( engine->window->window, mouse_motion_callback );

	// init button states so everything is considered "unpressed" at the start
	for( auto x = 0; x < input_id::max; ++x )
	{
		button_states[ x ] = false;
		button_states_last_frame[ x ] = false;
	}

	timer_repeat = std::make_unique<w_timer>( 150 );
}

void w_input::deinit()
{
	gamepad = nullptr;
}

void w_input::update()
{
	// mouse motion
	//
	// NOTE	- mouse motion deltas are sent once per update, not for each message from the OS
	//		- this is done to prevent tons of little messages from clogging up the works

	if( !fequals( mouse_move_delta.x + mouse_move_delta.y, 0.0f ) )
	{
		w_input_event evt;
		evt.event_id = event_id::input_motion;
		evt.input_id = input_id::mouse;
		evt.delta = mouse_move_delta;

		event_queue.emplace_back( std::move( evt ) );

		mouse_move_delta = w_vec2::zero;
	}

	// update button states

	button_states_last_frame = button_states;

	update_button_state( input_id::key_shift_left, glfwGetKey( engine->window->window, GLFW_KEY_LEFT_SHIFT ) );
	update_button_state( input_id::key_shift_right, glfwGetKey( engine->window->window, GLFW_KEY_RIGHT_SHIFT ) );
	update_button_state( input_id::key_control_left, glfwGetKey( engine->window->window, GLFW_KEY_LEFT_CONTROL ) );
	update_button_state( input_id::key_control_right, glfwGetKey( engine->window->window, GLFW_KEY_RIGHT_CONTROL ) );
	update_button_state( input_id::key_alt_left, glfwGetKey( engine->window->window, GLFW_KEY_LEFT_ALT ) );
	update_button_state( input_id::key_alt_right, glfwGetKey( engine->window->window, GLFW_KEY_RIGHT_ALT ) );
	update_button_state( input_id::mouse_button_left, glfwGetMouseButton( engine->window->window, GLFW_MOUSE_BUTTON_LEFT ) );
	update_button_state( input_id::mouse_button_middle, glfwGetMouseButton( engine->window->window, GLFW_MOUSE_BUTTON_MIDDLE ) );
	update_button_state( input_id::mouse_button_right, glfwGetMouseButton( engine->window->window, GLFW_MOUSE_BUTTON_RIGHT ) );
	update_button_state( input_id::key_esc, glfwGetKey( engine->window->window, GLFW_KEY_ESCAPE ) );
	update_button_state( input_id::key_pause, glfwGetKey( engine->window->window, GLFW_KEY_PAUSE ) );
	update_button_state( input_id::key_left_bracket, glfwGetKey( engine->window->window, GLFW_KEY_LEFT_BRACKET ) );
	update_button_state( input_id::key_right_bracket, glfwGetKey( engine->window->window, GLFW_KEY_RIGHT_BRACKET ) );
	update_button_state( input_id::key_enter, glfwGetKey( engine->window->window, GLFW_KEY_ENTER ) );
	update_button_state( input_id::key_space, glfwGetKey( engine->window->window, GLFW_KEY_SPACE ) );
	update_button_state( input_id::key_f1, glfwGetKey( engine->window->window, GLFW_KEY_F1 ) );
	update_button_state( input_id::key_f2, glfwGetKey( engine->window->window, GLFW_KEY_F2 ) );
	update_button_state( input_id::key_f3, glfwGetKey( engine->window->window, GLFW_KEY_F3 ) );
	update_button_state( input_id::key_f4, glfwGetKey( engine->window->window, GLFW_KEY_F4 ) );
	update_button_state( input_id::key_f5, glfwGetKey( engine->window->window, GLFW_KEY_F5 ) );
	update_button_state( input_id::key_f6, glfwGetKey( engine->window->window, GLFW_KEY_F6 ) );
	update_button_state( input_id::key_f7, glfwGetKey( engine->window->window, GLFW_KEY_F7 ) );
	update_button_state( input_id::key_f8, glfwGetKey( engine->window->window, GLFW_KEY_F8 ) );
	update_button_state( input_id::key_f9, glfwGetKey( engine->window->window, GLFW_KEY_F9 ) );
	update_button_state( input_id::key_f10, glfwGetKey( engine->window->window, GLFW_KEY_F10 ) );
	update_button_state( input_id::key_f11, glfwGetKey( engine->window->window, GLFW_KEY_F11 ) );
	update_button_state( input_id::key_f12, glfwGetKey( engine->window->window, GLFW_KEY_F12 ) );
	update_button_state( input_id::key_a, glfwGetKey( engine->window->window, GLFW_KEY_A ) );
	update_button_state( input_id::key_b, glfwGetKey( engine->window->window, GLFW_KEY_B ) );
	update_button_state( input_id::key_c, glfwGetKey( engine->window->window, GLFW_KEY_C ) );
	update_button_state( input_id::key_d, glfwGetKey( engine->window->window, GLFW_KEY_D ) );
	update_button_state( input_id::key_e, glfwGetKey( engine->window->window, GLFW_KEY_E ) );
	update_button_state( input_id::key_f, glfwGetKey( engine->window->window, GLFW_KEY_F ) );
	update_button_state( input_id::key_g, glfwGetKey( engine->window->window, GLFW_KEY_G ) );
	update_button_state( input_id::key_h, glfwGetKey( engine->window->window, GLFW_KEY_H ) );
	update_button_state( input_id::key_i, glfwGetKey( engine->window->window, GLFW_KEY_I ) );
	update_button_state( input_id::key_j, glfwGetKey( engine->window->window, GLFW_KEY_I ) );
	update_button_state( input_id::key_k, glfwGetKey( engine->window->window, GLFW_KEY_K ) );
	update_button_state( input_id::key_l, glfwGetKey( engine->window->window, GLFW_KEY_L ) );
	update_button_state( input_id::key_m, glfwGetKey( engine->window->window, GLFW_KEY_M ) );
	update_button_state( input_id::key_n, glfwGetKey( engine->window->window, GLFW_KEY_N ) );
	update_button_state( input_id::key_o, glfwGetKey( engine->window->window, GLFW_KEY_O ) );
	update_button_state( input_id::key_p, glfwGetKey( engine->window->window, GLFW_KEY_P ) );
	update_button_state( input_id::key_q, glfwGetKey( engine->window->window, GLFW_KEY_Q ) );
	update_button_state( input_id::key_r, glfwGetKey( engine->window->window, GLFW_KEY_R ) );
	update_button_state( input_id::key_s, glfwGetKey( engine->window->window, GLFW_KEY_S ) );
	update_button_state( input_id::key_t, glfwGetKey( engine->window->window, GLFW_KEY_T ) );
	update_button_state( input_id::key_u, glfwGetKey( engine->window->window, GLFW_KEY_U ) );
	update_button_state( input_id::key_v, glfwGetKey( engine->window->window, GLFW_KEY_V ) );
	update_button_state( input_id::key_w, glfwGetKey( engine->window->window, GLFW_KEY_W ) );
	update_button_state( input_id::key_x, glfwGetKey( engine->window->window, GLFW_KEY_X ) );
	update_button_state( input_id::key_y, glfwGetKey( engine->window->window, GLFW_KEY_Y ) );
	update_button_state( input_id::key_z, glfwGetKey( engine->window->window, GLFW_KEY_Z ) );
	update_button_state( input_id::key_0, glfwGetKey( engine->window->window, GLFW_KEY_0 ) );
	update_button_state( input_id::key_1, glfwGetKey( engine->window->window, GLFW_KEY_1 ) );
	update_button_state( input_id::key_2, glfwGetKey( engine->window->window, GLFW_KEY_2 ) );
	update_button_state( input_id::key_3, glfwGetKey( engine->window->window, GLFW_KEY_3 ) );
	update_button_state( input_id::key_4, glfwGetKey( engine->window->window, GLFW_KEY_4 ) );
	update_button_state( input_id::key_5, glfwGetKey( engine->window->window, GLFW_KEY_5 ) );
	update_button_state( input_id::key_6, glfwGetKey( engine->window->window, GLFW_KEY_6 ) );
	update_button_state( input_id::key_7, glfwGetKey( engine->window->window, GLFW_KEY_7 ) );
	update_button_state( input_id::key_8, glfwGetKey( engine->window->window, GLFW_KEY_8 ) );
	update_button_state( input_id::key_9, glfwGetKey( engine->window->window, GLFW_KEY_9 ) );
	update_button_state( input_id::key_up, glfwGetKey( engine->window->window, GLFW_KEY_UP ) );
	update_button_state( input_id::key_down, glfwGetKey( engine->window->window, GLFW_KEY_DOWN ) );
	update_button_state( input_id::key_left, glfwGetKey( engine->window->window, GLFW_KEY_LEFT ) );
	update_button_state( input_id::key_right, glfwGetKey( engine->window->window, GLFW_KEY_RIGHT ) );
	update_button_state( input_id::key_period, glfwGetKey( engine->window->window, GLFW_KEY_PERIOD ) );
	update_button_state( input_id::key_comma, glfwGetKey( engine->window->window, GLFW_KEY_COMMA ) );
	update_button_state( input_id::key_slash, glfwGetKey( engine->window->window, GLFW_KEY_SLASH ) );
	update_button_state( input_id::key_tilde, glfwGetKey( engine->window->window, GLFW_KEY_GRAVE_ACCENT ) );
	update_button_state( input_id::key_delete, glfwGetKey( engine->window->window, GLFW_KEY_DELETE ) );

	// update game controller states

	if( gamepad )
	{
		gamepad->update();

		// update state information and queue events

		gamepad->update_button_state( input_id::gamepad_button_a, XINPUT_GAMEPAD_A );
		gamepad->update_button_state( input_id::gamepad_button_b, XINPUT_GAMEPAD_B );
		gamepad->update_button_state( input_id::gamepad_button_x, XINPUT_GAMEPAD_X );
		gamepad->update_button_state( input_id::gamepad_button_y, XINPUT_GAMEPAD_Y );
		gamepad->update_button_state( input_id::gamepad_button_dpad_left, XINPUT_GAMEPAD_DPAD_LEFT );
		gamepad->update_button_state( input_id::gamepad_button_dpad_right, XINPUT_GAMEPAD_DPAD_RIGHT );
		gamepad->update_button_state( input_id::gamepad_button_dpad_up, XINPUT_GAMEPAD_DPAD_UP );
		gamepad->update_button_state( input_id::gamepad_button_dpad_down, XINPUT_GAMEPAD_DPAD_DOWN );
		gamepad->update_button_state( input_id::gamepad_button_left_thumb, XINPUT_GAMEPAD_LEFT_THUMB );
		gamepad->update_button_state( input_id::gamepad_button_right_thumb, XINPUT_GAMEPAD_RIGHT_THUMB );
		gamepad->update_button_state( input_id::gamepad_button_left_shoulder, XINPUT_GAMEPAD_LEFT_SHOULDER );
		gamepad->update_button_state( input_id::gamepad_button_right_shoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER );

		auto update_axis_delta = [] ( e_input_id input_id )
		{
			w_vec2 delta = engine->input->get_axis_state( input_id );

			if( !delta.is_zero() )
			{
				w_input_event evt;
				evt.event_id = event_id::input_motion;
				evt.input_id = input_id;
				evt.delta = delta;

				engine->input->event_queue.emplace_back( std::move( evt ) );
			}
		};

		update_axis_delta( input_id::gamepad_left_stick );
		update_axis_delta( input_id::gamepad_right_stick );
		update_axis_delta( input_id::gamepad_left_trigger );
		update_axis_delta( input_id::gamepad_right_trigger );
	}

	// send every accumulated input message to anyone listening

	for( auto& evt : event_queue )
	{
		switch( evt.event_id )
		{
			case event_id::input_pressed:
			{
				if( engine->iir_on_pressed( &evt ) )
				{
					break;
				}

				if( engine->layer_mgr->iir_on_pressed( &evt ) )
				{
					break;
				}
			}
			break;

			case event_id::input_held:
			{
				if( engine->iir_on_held( &evt ) )
				{
					break;
				}

				if( engine->layer_mgr->iir_on_held( &evt ) )
				{
					break;
				}
			}
			break;

			case event_id::input_released:
			{
				if( engine->iir_on_released( &evt ) )
				{
					break;
				}

				if( engine->layer_mgr->iir_on_released( &evt ) )
				{
					break;
				}
			}
			break;

			case event_id::input_motion:
			{
				if( engine->iir_on_motion( &evt ) )
				{
					break;
				}

				if( engine->layer_mgr->iir_on_motion( &evt ) )
				{
					break;
				}
			}
			break;
		}
	}

	assert( event_queue.size() < event_queue_max_size );
	event_queue = {};
}

void w_input::update_button_state( e_input_id input_id, int glfw_state )
{
	button_states[ input_id ] = glfw_state;

	e_button_state bs = get_button_state( input_id );

	switch( bs )
	{
		case button_state::pressed:
		{
			w_input_event evt;
			evt.event_id = event_id::input_pressed;
			evt.input_id = input_id;

			engine->input->event_queue.emplace_back( std::move( evt ) );

			timer_repeat->reset();
		}
		break;

		case button_state::released:
		{
			w_input_event evt;
			evt.event_id = event_id::input_released;
			evt.input_id = input_id;

			engine->input->event_queue.emplace_back( std::move( evt ) );
		}
		break;

		case button_state::held:
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
		break;
	}
}

void w_input::play_rumble( e_rumble_effect effect )
{
	if( !gamepad || !gamepad->is_being_used )
	{
		return;
	}

	gamepad->play_rumble( effect );
}

void w_input::refresh_gamepad()
{
	if( gamepad )
	{
		gamepad->play_rumble( rumble_effect::none );
	}

	gamepad = nullptr;

	// look for an attached xbox controller. the first valid one
	// we find is the one we use for player input.

	XINPUT_STATE state;
	ZeroMemory( &state, sizeof( XINPUT_STATE ) );

	int xinput_player_id = -1;
	for( int pn = 0 ; pn < XUSER_MAX_COUNT && xinput_player_id == -1 ; ++pn )
	{
		if( XInputGetState( pn, &state ) == ERROR_SUCCESS )
		{
			xinput_player_id = pn;

			log( "Using controller : player_id : {}", xinput_player_id );
			gamepad = std::make_unique<w_gamepad>( xinput_player_id );
			break;
		}
	}
}

bool w_input::is_button_down( e_input_id input_id )
{
	e_button_state bs = get_button_state( input_id );
	return ( bs == button_state::pressed || bs == button_state::held );
}

bool w_input::is_shift_down()
{
	e_button_state bs_left = get_button_state( input_id::key_shift_left );
	e_button_state bs_right = get_button_state( input_id::key_shift_right );

	return (
		bs_left == button_state::pressed
		|| bs_left == button_state::held
		|| bs_right == button_state::pressed
		|| bs_right == button_state::held
	);
}

bool w_input::is_control_down()
{
	e_button_state bs_left = get_button_state( input_id::key_control_left );
	e_button_state bs_right = get_button_state( input_id::key_control_right );

	return (
		bs_left == button_state::pressed
		|| bs_left == button_state::held
		|| bs_right == button_state::pressed
		|| bs_right == button_state::held
	);
}

bool w_input::is_alt_down()
{
	e_button_state bs_left = get_button_state( input_id::key_alt_left );
	e_button_state bs_right = get_button_state( input_id::key_alt_right );

	return (
		bs_left == button_state::pressed
		|| bs_left == button_state::held
		|| bs_right == button_state::pressed
		|| bs_right == button_state::held
	);
}

e_button_state w_input::get_button_state( e_input_id input_id )
{
	e_button_state bs = button_state::up;

	bool state = button_states[ input_id ];
	bool state_last_frame = button_states_last_frame[ input_id ];

	if( state && state_last_frame )
	{
		bs = button_state::held;
	}
	else if( state && !state_last_frame )
	{
		bs = button_state::pressed;
	}
	else if( !state && state_last_frame )
	{
		bs = button_state::released;
	}

	return bs;
}

/*
	returns a value between -1.0/+1.0 for the requested axis.

	these values are updated once per frame.
*/

constexpr float gamepad_dead_zone = 0.20f;
constexpr float gamepad_dead_zone_small = 0.10f;

w_vec2 w_input::get_axis_state( e_input_id input_id, bool ignore_dead_zone )
{
	assert( gamepad );

	float dead_zone = ignore_dead_zone ? gamepad_dead_zone_small : gamepad_dead_zone;

	//game_controller->update_state();

	w_vec2 value;

	switch( input_id )
	{
		case input_id::gamepad_left_stick:
		{
			value.x = glm::max( -1.0f, (float) gamepad->xinput_state.Gamepad.sThumbLX / 32767.0f );
			value.y = glm::max( -1.0f, (float) gamepad->xinput_state.Gamepad.sThumbLY / 32767.0f ) * -1.0f;

			if( glm::abs( value.x ) < dead_zone )
			{
				value.x = 0.f;
			}
			if( glm::abs( value.y ) < dead_zone )
			{
				value.y = 0.f;
			}
		}
		break;

		case input_id::gamepad_right_stick:
		{
			value.x = glm::max( -1.0f, (float) gamepad->xinput_state.Gamepad.sThumbRX / 32767.0f );
			value.y = glm::max( -1.0f, (float) gamepad->xinput_state.Gamepad.sThumbRY / 32767.0f ) * -1.0f;

			if( glm::abs( value.x ) < dead_zone || glm::abs( value.y ) < dead_zone )
			{
				value.x = 0.f;
				value.y = 0.f;
			}
		}
		break;

		case input_id::gamepad_left_trigger:
		{
			value.x = gamepad->xinput_state.Gamepad.bLeftTrigger / 255.0f;
		}
		break;

		case input_id::gamepad_right_trigger:
		{
			value.x = gamepad->xinput_state.Gamepad.bRightTrigger / 255.0f;
		}
		break;

		default:
		{
			log_error( "Unknown axis" );
		}
		break;
	}

	// note : the value being returned is normalized
	return value;
}
