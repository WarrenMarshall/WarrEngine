
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

input_event::input_event()
{
	shift_down = g_engine->input.is_shift_down();
	control_down = g_engine->input.is_control_down();
	alt_down = g_engine->input.is_alt_down();

	mouse_pos = g_engine->input.mouse_window_pos;
}

// ----------------------------------------------------------------------------

void character_callback( GLFWwindow* window, unsigned int key_code )
{
	input_event evt;
	evt.event_id = event_id::input_key;
	evt.ch = (unsigned char)key_code;

	g_engine->input.event_queue.emplace_back( std::move( evt ) );
}

// ----------------------------------------------------------------------------

void mouse_wheel_callback( GLFWwindow* window, double xoffset, double yoffset )
{
	g_engine->input.mouse_wheel_delta.x += (float)( xoffset );
	g_engine->input.mouse_wheel_delta.y += (float)( yoffset );
}

// ----------------------------------------------------------------------------

static vec2 last_mouse_pos = vec2::zero;

void mouse_motion_callback( GLFWwindow* window, double xpos, double ypos )
{
	auto fxpos = (float)xpos;
	auto fypos = (float)ypos;

	// compute movement delta in window space
	g_engine->input.mouse_move_delta.x += fxpos - last_mouse_pos.x;
	g_engine->input.mouse_move_delta.y += fypos - last_mouse_pos.y;

	last_mouse_pos = vec2( fxpos, fypos );

	// save the window space position of the mouse
	g_engine->input.mouse_window_pos.x = fxpos;
	g_engine->input.mouse_window_pos.y = fypos;
}

// ----------------------------------------------------------------------------

void joystick_callback( int jid, int event )
{
	if( event == GLFW_CONNECTED )
	{
		log_verbose( "Gamepad {} was connected.", jid );
	}
	else if( event == GLFW_DISCONNECTED )
	{
		log_verbose( "Gamepad {} was disconnected.", jid );
	}

	g_engine->input.refresh_connected_gamepads();
}

// ----------------------------------------------------------------------------

void input_mgr::init()
{
	refresh_connected_gamepads();

	// set up callback so we know when controllers are connected/disconnected
	glfwSetJoystickCallback( joystick_callback );

	// callbacks so we can collect user input
	glfwSetCursorPosCallback( g_engine->window.glfw_window, mouse_motion_callback );
	glfwSetScrollCallback( g_engine->window.glfw_window, mouse_wheel_callback );

	// keyboard input
	glfwSetCharCallback( g_engine->window.glfw_window, character_callback );

	// init button states so everything is considered "unpressed" at the start
	for( auto x = 0; x < input_id::max; ++x )
	{
		button_states[ x ] = false;
		button_states_last_frame[ x ] = false;
	}

	timer_repeat = std::make_unique<timer>( 150 );
}

void input_mgr::deinit()
{
	gamepad = nullptr;
}

void input_mgr::queue_presses()
{
	button_states_last_frame = button_states;

	update_button_state( input_id::key_backspace, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_BACKSPACE ) );
	update_button_state( input_id::key_shift_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_SHIFT ) );
	update_button_state( input_id::key_shift_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_SHIFT ) );
	update_button_state( input_id::key_control_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_CONTROL ) );
	update_button_state( input_id::key_control_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_CONTROL ) );
	update_button_state( input_id::key_alt_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_ALT ) );
	update_button_state( input_id::key_alt_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_ALT ) );
	update_button_state( input_id::mouse_button_left, glfwGetMouseButton( g_engine->window.glfw_window, GLFW_MOUSE_BUTTON_LEFT ) );
	update_button_state( input_id::mouse_button_middle, glfwGetMouseButton( g_engine->window.glfw_window, GLFW_MOUSE_BUTTON_MIDDLE ) );
	update_button_state( input_id::mouse_button_right, glfwGetMouseButton( g_engine->window.glfw_window, GLFW_MOUSE_BUTTON_RIGHT ) );
	update_button_state( input_id::key_esc, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_ESCAPE ) );
	update_button_state( input_id::key_pause, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PAUSE ) );
	update_button_state( input_id::key_left_bracket, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_BRACKET ) );
	update_button_state( input_id::key_right_bracket, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_BRACKET ) );
	update_button_state( input_id::key_enter, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_ENTER ) );
	update_button_state( input_id::key_space, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_SPACE ) );
	update_button_state( input_id::key_f1, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F1 ) );
	update_button_state( input_id::key_f2, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F2 ) );
	update_button_state( input_id::key_f3, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F3 ) );
	update_button_state( input_id::key_f4, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F4 ) );
	update_button_state( input_id::key_f5, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F5 ) );
	update_button_state( input_id::key_f6, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F6 ) );
	update_button_state( input_id::key_f7, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F7 ) );
	update_button_state( input_id::key_f8, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F8 ) );
	update_button_state( input_id::key_f9, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F9 ) );
	update_button_state( input_id::key_f10, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F10 ) );
	update_button_state( input_id::key_f11, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F11 ) );
	update_button_state( input_id::key_f12, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F12 ) );
	update_button_state( input_id::key_a, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_A ) );
	update_button_state( input_id::key_b, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_B ) );
	update_button_state( input_id::key_c, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_C ) );
	update_button_state( input_id::key_d, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_D ) );
	update_button_state( input_id::key_e, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_E ) );
	update_button_state( input_id::key_f, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F ) );
	update_button_state( input_id::key_g, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_G ) );
	update_button_state( input_id::key_h, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_H ) );
	update_button_state( input_id::key_i, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_I ) );
	update_button_state( input_id::key_j, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_I ) );
	update_button_state( input_id::key_k, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_K ) );
	update_button_state( input_id::key_l, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_L ) );
	update_button_state( input_id::key_m, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_M ) );
	update_button_state( input_id::key_n, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_N ) );
	update_button_state( input_id::key_o, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_O ) );
	update_button_state( input_id::key_p, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_P ) );
	update_button_state( input_id::key_q, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_Q ) );
	update_button_state( input_id::key_r, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_R ) );
	update_button_state( input_id::key_s, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_S ) );
	update_button_state( input_id::key_t, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_T ) );
	update_button_state( input_id::key_u, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_U ) );
	update_button_state( input_id::key_v, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_V ) );
	update_button_state( input_id::key_w, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_W ) );
	update_button_state( input_id::key_x, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_X ) );
	update_button_state( input_id::key_y, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_Y ) );
	update_button_state( input_id::key_z, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_Z ) );
	update_button_state( input_id::key_0, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_0 ) );
	update_button_state( input_id::key_1, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_1 ) );
	update_button_state( input_id::key_2, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_2 ) );
	update_button_state( input_id::key_3, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_3 ) );
	update_button_state( input_id::key_4, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_4 ) );
	update_button_state( input_id::key_5, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_5 ) );
	update_button_state( input_id::key_6, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_6 ) );
	update_button_state( input_id::key_7, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_7 ) );
	update_button_state( input_id::key_8, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_8 ) );
	update_button_state( input_id::key_9, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_9 ) );
	update_button_state( input_id::key_up, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_UP ) );
	update_button_state( input_id::key_down, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_DOWN ) );
	update_button_state( input_id::key_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT ) );
	update_button_state( input_id::key_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT ) );
	update_button_state( input_id::key_period, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PERIOD ) );
	update_button_state( input_id::key_comma, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_COMMA ) );
	update_button_state( input_id::key_slash, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_SLASH ) );
	update_button_state( input_id::key_tilde, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_GRAVE_ACCENT ) );
	update_button_state( input_id::key_insert, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_INSERT ) );
	update_button_state( input_id::key_delete, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_DELETE ) );
	update_button_state( input_id::key_home, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_HOME ) );
	update_button_state( input_id::key_end, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_END ) );
	update_button_state( input_id::key_page_up, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PAGE_UP ) );
	update_button_state( input_id::key_page_down, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PAGE_DOWN ) );

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
		gamepad->update_button_state( input_id::gamepad_button_start, XINPUT_GAMEPAD_START );
		gamepad->update_button_state( input_id::gamepad_button_back, XINPUT_GAMEPAD_BACK );
	}
}

void input_mgr::queue_motion()
{
	// mouse motion
	//
	// NOTE: mouse motion deltas are sent once per update, not for each message
	// from the OS. this is done to prevent tons of little messages from
	// clogging up the works.

	// window

	if( !fequals( mouse_move_delta.x + mouse_move_delta.y, 0.f ) )
	{
		input_event evt;
		evt.event_id = event_id::input_motion;
		evt.input_id = input_id::mouse;
		evt.delta = mouse_move_delta;

		event_queue.emplace_back( std::move( evt ) );

		mouse_move_delta = vec2::zero;
	}

	// mouse wheel

	if( !fequals( mouse_wheel_delta.x + mouse_wheel_delta.y, 0.f ) )
	{
		input_event evt;
		evt.event_id = event_id::input_motion;
		evt.input_id = input_id::mouse_wheel;
		evt.delta = mouse_wheel_delta;

		event_queue.emplace_back( std::move( evt ) );

		mouse_wheel_delta = vec2::zero;
	}

	// game controller

	if( gamepad )
	{
		gamepad->update();

		update_axis_delta( input_id::gamepad_left_stick );
		update_axis_delta( input_id::gamepad_right_stick );
		update_axis_delta( input_id::gamepad_left_trigger );
		update_axis_delta( input_id::gamepad_right_trigger );
	}
}

void input_mgr::update_axis_delta( e_input_id input_id ) const
{
	vec2 delta = g_engine->input.get_axis_state( input_id );

	if( !delta.is_zero() )
	{
		input_event evt;
		evt.event_id = event_id::input_motion;
		evt.input_id = input_id;
		evt.delta = delta;

		g_engine->input.event_queue.emplace_back( std::move( evt ) );
	}
}

void input_mgr::update()
{
	// send all queued messages down the virtual function call chain,
	// and then empty the queue.

	for( auto& evt : event_queue )
	{
		switch( evt.event_id )
		{
			case event_id::input_pressed:
			{
				if( g_engine->scenes.on_input_pressed( &evt ) )
				{
					break;
				}

				if( g_engine->on_input_pressed( &evt ) )
				{
					break;
				}

				break;
			}

			case event_id::input_held:
			{
				if( g_engine->scenes.on_input_held( &evt ) )
				{
					break;
				}

				if( g_engine->on_input_held( &evt ) )
				{
					break;
				}
				break;
			}

			case event_id::input_released:
			{
				if( g_engine->scenes.on_input_released( &evt ) )
				{
					break;
				}

				if( g_engine->on_input_released( &evt ) )
				{
					break;
				}
				break;
			}

			case event_id::input_motion:
			{
				if( g_engine->scenes.on_input_motion( &evt ) )
				{
					break;
				}

				if( g_engine->on_input_motion( &evt ) )
				{
					break;
				}
				break;
			}

			case event_id::input_key:
			{
				if( g_engine->scenes.on_input_key( &evt ) )
				{
					break;
				}

				if( g_engine->on_input_key( &evt ) )
				{
					break;
				}
				break;
			}
		}
	}

	event_queue.clear();
}

void input_mgr::update_button_state( e_input_id input_id, int glfw_state )
{
	button_states[ input_id ] = glfw_state;

	switch( get_button_state( input_id ) )
	{
		case button_state::pressed:
		{
			input_event evt;
			evt.event_id = event_id::input_pressed;
			evt.input_id = input_id;

			g_engine->input.event_queue.emplace_back( std::move( evt ) );

			timer_repeat->restart();
			break;
		}

		case button_state::released:
		{
			input_event evt;
			evt.event_id = event_id::input_released;
			evt.input_id = input_id;

			g_engine->input.event_queue.emplace_back( std::move( evt ) );
			break;
		}

		case button_state::held:
		{
			if( timer_repeat->get_elapsed() )
			{
				input_event evt;
				evt.event_id = event_id::input_held;
				evt.input_id = input_id;

				g_engine->input.event_queue.emplace_back( std::move( evt ) );

				timer_repeat->restart();
				g_ui->caret_blink_tween.restart();
			}
			break;
		}
	}
}

void input_mgr::play_rumble( e_rumble_effect effect ) const
{
	if( !gamepad or !gamepad->is_being_used )
	{
		return;
	}

	gamepad->play_rumble( effect );
}

void input_mgr::refresh_connected_gamepads()
{
	auto save_gamepad = gamepad.get();

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
	for( auto pn = 0 ; pn < XUSER_MAX_COUNT and xinput_player_id == -1 ; ++pn )
	{
		if( XInputGetState( pn, &state ) == ERROR_SUCCESS )
		{
			xinput_player_id = pn;

			log( "Using controller : player_id : {}", xinput_player_id );
			gamepad = std::make_unique<game_controller>( xinput_player_id );
			break;
		}
	}

	// if a gamepad is newly detected, give it a little rumble
	if( gamepad and gamepad.get() != save_gamepad )
	{
		gamepad->play_rumble( rumble_effect::large );
	}
}

bool input_mgr::is_button_down( e_input_id input_id )
{
	e_button_state bs = get_button_state( input_id );
	return ( bs == button_state::pressed or bs == button_state::held );
}

bool input_mgr::is_shift_down()
{
	e_button_state bs_left = get_button_state( input_id::key_shift_left );
	e_button_state bs_right = get_button_state( input_id::key_shift_right );

	return (
		bs_left == button_state::pressed
		or bs_left == button_state::held
		or bs_right == button_state::pressed
		or bs_right == button_state::held
		);
}

bool input_mgr::is_control_down()
{
	e_button_state bs_left = get_button_state( input_id::key_control_left );
	e_button_state bs_right = get_button_state( input_id::key_control_right );

	return (
		bs_left == button_state::pressed
		or bs_left == button_state::held
		or bs_right == button_state::pressed
		or bs_right == button_state::held
		);
}

bool input_mgr::is_alt_down()
{
	e_button_state bs_left = get_button_state( input_id::key_alt_left );
	e_button_state bs_right = get_button_state( input_id::key_alt_right );

	return (
		bs_left == button_state::pressed
		or bs_left == button_state::held
		or bs_right == button_state::pressed
		or bs_right == button_state::held
		);
}

// determines the state of a button by comparing it's current state with the
// previous frame.

e_button_state input_mgr::get_button_state( e_input_id input_id )
{
	e_button_state bs = button_state::up;

	bool state = button_states[ input_id ];
	bool state_last_frame = button_states_last_frame[ input_id ];

	if( state and state_last_frame )
	{
		bs = button_state::held;
	}
	else if( state and !state_last_frame )
	{
		bs = button_state::pressed;
	}
	else if( !state and state_last_frame )
	{
		bs = button_state::released;
	}

	return bs;
}

// returns a value between -1.0/+1.0 for the requested axis.
//
// these values are updated once per frame.

// #task - should be in the config INI file?
constexpr float gamepad_dead_zone = 0.25f;

vec2 input_mgr::get_axis_state( e_input_id input_id, bool use_dead_zone )
{
	if( !gamepad )
	{
		return vec2::zero;
	}

	vec2 value;

	switch( input_id )
	{
		case input_id::gamepad_left_stick:
		{
			value.x = glm::max( -1.f, (float)gamepad->xinput_state.Gamepad.sThumbLX / 32767.f );
			value.y = glm::max( -1.f, (float)gamepad->xinput_state.Gamepad.sThumbLY / 32767.f ) * -1.f;

			if( use_dead_zone )
			{
				auto dist = glm::abs( value.get_size_squared() );
				if( dist < gamepad_dead_zone )
				{
					value.x = value.y = 0.f;
				}
			}
			break;
		}

		case input_id::gamepad_right_stick:
		{
			value.x = glm::max( -1.f, (float)gamepad->xinput_state.Gamepad.sThumbRX / 32767.f );
			value.y = glm::max( -1.f, (float)gamepad->xinput_state.Gamepad.sThumbRY / 32767.f ) * -1.f;

			if( use_dead_zone )
			{
				auto dist = glm::abs( value.get_size_squared() );
				if( dist < gamepad_dead_zone )
				{
					value.x = value.y = 0.f;
				}
			}
			break;
		}

		case input_id::gamepad_left_trigger:
		{
			value.x = value.y = gamepad->xinput_state.Gamepad.bLeftTrigger / 255.f;
			break;
		}

		case input_id::gamepad_right_trigger:
		{
			value.x = value.y = gamepad->xinput_state.Gamepad.bRightTrigger / 255.f;
			break;
		}

		default:
		{
			log_fatal( "Unknown axis" );
			break;
		}
	}

	// note : the value being returned is normalized
	return value;
}

}
