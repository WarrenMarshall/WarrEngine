
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

	//glfwSetCursorPos( engine->window->window, v_window_hw, v_window_hh );
}

// ----------------------------------------------------------------------------

void w_input::init()
{
	// look for an attached xbox controller

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if( XInputGetState( 0, &state ) == ERROR_SUCCESS )
	{
		// controller is connected on port 1
		log_msg( "Using controller : {}", 0 );
		game_controller = std::make_unique<w_game_controller>( 0 );
	}

	// callbacks so we can collect user input
	glfwSetCursorPosCallback( engine->window->window, mouse_motion_callback );

	for( int x = 0; x < input_id::max; ++x )
	{
		button_states[ x ] = false;
		button_states_last_frame[ x ] = false;
	}

	glfw_codes.insert( std::make_pair( GLFW_KEY_ESCAPE, input_id::key_esc ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_PAUSE, input_id::key_pause ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_LEFT_BRACKET, input_id::key_left_bracket ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_RIGHT_BRACKET, input_id::key_right_bracket ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_ENTER, input_id::key_enter ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_SPACE, input_id::key_space ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F1, input_id::key_f1 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F2, input_id::key_f2 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F3, input_id::key_f3 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F4, input_id::key_f4 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F5, input_id::key_f5 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F6, input_id::key_f6 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F7, input_id::key_f7 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F8, input_id::key_f8 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F9, input_id::key_f9 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F10, input_id::key_f10 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F11, input_id::key_f11 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F12, input_id::key_f12 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_A, input_id::key_a ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_B, input_id::key_b ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_C, input_id::key_c ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_D, input_id::key_d ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_E, input_id::key_e ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F, input_id::key_f ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_G, input_id::key_g ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_H, input_id::key_h ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_I, input_id::key_i ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_I, input_id::key_j ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_K, input_id::key_k ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_L, input_id::key_l ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_M, input_id::key_m ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_N, input_id::key_n ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_O, input_id::key_o ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_P, input_id::key_p ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_Q, input_id::key_q ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_R, input_id::key_r ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_S, input_id::key_s ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_T, input_id::key_t ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_U, input_id::key_u ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_V, input_id::key_v ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_W, input_id::key_w ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_X, input_id::key_x ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_Y, input_id::key_y ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_Z, input_id::key_z ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_0, input_id::key_0 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_1, input_id::key_1 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_2, input_id::key_2 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_3, input_id::key_3 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_4, input_id::key_4 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_5, input_id::key_5 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_6, input_id::key_6 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_7, input_id::key_7 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_8, input_id::key_8 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_9, input_id::key_9 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_UP, input_id::key_up ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_DOWN, input_id::key_down ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_LEFT, input_id::key_left ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_RIGHT, input_id::key_right ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_PERIOD, input_id::key_period ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_COMMA, input_id::key_comma ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_SLASH, input_id::key_slash ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_GRAVE_ACCENT, input_id::key_tilde ) );
	glfw_codes.insert( std::make_pair( GLFW_MOUSE_BUTTON_LEFT, input_id::mouse_button_left ) );
	glfw_codes.insert( std::make_pair( GLFW_MOUSE_BUTTON_MIDDLE, input_id::mouse_button_middle ) );
	glfw_codes.insert( std::make_pair( GLFW_MOUSE_BUTTON_RIGHT, input_id::mouse_button_right ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_LEFT_SHIFT, input_id::key_shift_left ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_RIGHT_SHIFT, input_id::key_shift_right ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_LEFT_CONTROL, input_id::key_control_left ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_RIGHT_CONTROL, input_id::key_control_right ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_LEFT_ALT, input_id::key_alt_left ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_RIGHT_ALT, input_id::key_alt_right ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_DELETE, input_id::key_delete ) );

	timer_repeat = std::make_unique<w_timer>( 150 );
}

void w_input::deinit()
{
	game_controller = nullptr;
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
		evt.mouse.delta = mouse_move_delta;

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

	if( game_controller )
	{
		game_controller->update();
	}

	// send every accumulated input message to anyone listening

	for( auto& evt : event_queue )
	{
		send_event_to_listeners( evt.event_id, &evt );
	}

	event_queue.clear();
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

			//is_being_used = true;
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
				evt.event_id = event_id::input_pressed;
				evt.input_id = input_id;

				engine->input->event_queue.emplace_back( std::move( evt ) );
			}
		}
		break;
	}
}

void w_input::play_rumble( e_rumble_effect effect )
{
	if( !game_controller || !game_controller->is_being_used )
	{
		return;
	}

	game_controller->play_rumble( effect );
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

	return ( bs_left == button_state::pressed || bs_left == button_state::held || bs_right == button_state::pressed || bs_right == button_state::held );
}

bool w_input::is_control_down()
{
	e_button_state bs_left = get_button_state( input_id::key_control_left );
	e_button_state bs_right = get_button_state( input_id::key_control_right );

	return ( bs_left == button_state::pressed || bs_left == button_state::held || bs_right == button_state::pressed || bs_right == button_state::held );
}

bool w_input::is_alt_down()
{
	e_button_state bs_left = get_button_state( input_id::key_alt_left );
	e_button_state bs_right = get_button_state( input_id::key_alt_right );

	return ( bs_left == button_state::pressed || bs_left == button_state::held || bs_right == button_state::pressed || bs_right == button_state::held );
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

static float controller_dead_zone = 0.15f;
w_vec2 w_input::axis_value_of( e_input_id input_id )
{
	if( !game_controller )
	{
		return w_vec2::zero;
	}

	game_controller->update();
	w_vec2 value;

	switch( input_id )
	{
		case input_id::controller_left_stick:
		{
			value.x = w_max( -1.0f, (float) game_controller->xinput_state.Gamepad.sThumbLX / 32767.0f );
			value.y = w_max( -1.0f, (float) game_controller->xinput_state.Gamepad.sThumbLY / 32767.0f ) * -1.0f;

			if( fabs( value.x ) < controller_dead_zone )
			{
				value.x = 0.f;
			}
			if( fabs( value.y ) < controller_dead_zone )
			{
				value.y = 0.f;
			}
		}
		break;

		case input_id::controller_right_stick:
		{
			value.x = w_max( -1.0f, (float) game_controller->xinput_state.Gamepad.sThumbRX / 32767.0f );
			value.y = w_max( -1.0f, (float) game_controller->xinput_state.Gamepad.sThumbRY / 32767.0f ) * -1.0f;

			if( fabs( value.x ) < controller_dead_zone )
			{
				value.x = 0.f;
			}
			if( fabs( value.y ) < controller_dead_zone )
			{
				value.y = 0.f;
			}
		}
		break;

		case input_id::controller_left_trigger:
		{
			value.x = game_controller->xinput_state.Gamepad.bLeftTrigger / 255.0f;
		}
		break;

		case input_id::controller_right_trigger:
		{
			value.x = game_controller->xinput_state.Gamepad.bRightTrigger / 255.0f;
		}
		break;

		default:
		{
			log_error( "Unknown axis" );
		}
		break;
	}

	// note : the value being returned should be normalized
	return value;
}
