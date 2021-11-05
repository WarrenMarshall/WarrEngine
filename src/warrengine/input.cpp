
#include "master_pch.h"

namespace war
{

// ----------------------------------------------------------------------------

Input_Event::Input_Event()
{
	shift_down = g_engine->input_mgr.is_shift_down();
	control_down = g_engine->input_mgr.is_control_down();
	alt_down = g_engine->input_mgr.is_alt_down();

	mouse_pos = g_engine->input_mgr.mouse_window_pos;
}

Input_Event::Input_Event( e_event_id event_id, e_input_id input_id )
	: event_id( event_id ), input_id( input_id )
{
	shift_down = g_engine->input_mgr.is_shift_down();
	control_down = g_engine->input_mgr.is_control_down();
	alt_down = g_engine->input_mgr.is_alt_down();

	mouse_pos = g_engine->input_mgr.mouse_window_pos;
}

// ----------------------------------------------------------------------------

void character_callback( GLFWwindow* window, ui32 key_code )
{
	Input_Event evt;
	evt.event_id = e_event_id::input_key;
	evt.ch = (ui8)key_code;

	g_engine->input_mgr.event_queue.push_back( evt );
}

// ----------------------------------------------------------------------------

void mouse_wheel_callback( GLFWwindow* window, d64 xoffset, d64 yoffset )
{
	g_engine->input_mgr.mouse_wheel_delta.x += (f32)( xoffset );
	g_engine->input_mgr.mouse_wheel_delta.y += (f32)( yoffset );
}

// ----------------------------------------------------------------------------

static Vec2 last_mouse_pos = Vec2::zero;

void mouse_motion_callback( GLFWwindow* window, d64 xpos, d64 ypos )
{
	auto fxpos = (f32)xpos;
	auto fypos = (f32)ypos;

	// compute movement delta in window space
	g_engine->input_mgr.mouse_move_delta.x += fxpos - last_mouse_pos.x;
	g_engine->input_mgr.mouse_move_delta.y += fypos - last_mouse_pos.y;

	last_mouse_pos = Vec2( fxpos, fypos );

	// save the window space position of the mouse
	g_engine->input_mgr.mouse_window_pos.x = fxpos;
	g_engine->input_mgr.mouse_window_pos.y = fypos;
}

// ----------------------------------------------------------------------------

void joystick_callback( i32 jid, i32 event )
{
	if( event == GLFW_CONNECTED )
	{
		log_verbose( "Gamepad {} was connected.", jid );
	}
	else if( event == GLFW_DISCONNECTED )
	{
		log_verbose( "Gamepad {} was disconnected.", jid );
	}

	g_engine->input_mgr.refresh_connected_gamepads();
}

// ----------------------------------------------------------------------------

void Input_Mgr::init()
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
	for( auto x = 0; x < (i32)e_input_id::max; ++x )
	{
		button_states[ x ] = false;
		button_states_last_frame[ x ] = false;
	}

	timer_repeat = Timer( 150 );

	// grouped key bindings

	group_bindings.insert( std::make_pair( H( "left" ), e_input_id::gamepad_button_dpad_left ) );
	group_bindings.insert( std::make_pair( H( "left" ), e_input_id::key_left ) );
	group_bindings.insert( std::make_pair( H( "left" ), e_input_id::gamepad_left_stick ) );
}

void Input_Mgr::deinit()
{
	gamepad = std::nullopt;
}

void Input_Mgr::queue_presses()
{
	if( g_engine->is_paused() )
	{
		// if the engine is currently paused, the only certain keypresses will
		// be acknowledged so we can unpause the game as well as use the pause
		// state for inspection/debugging.

		button_states_last_frame[ ( i32 )e_input_id::key_pause ] = button_states[ ( i32 )e_input_id::key_pause ];

#ifndef _FINAL_RELEASE
		button_states_last_frame[ ( i32 )e_input_id::mouse_button_middle ] = button_states[ ( i32 )e_input_id::mouse_button_middle ];
		button_states_last_frame[ ( i32 )e_input_id::key_f5 ] = button_states[ ( i32 )e_input_id::key_f5 ];
		button_states_last_frame[ ( i32 )e_input_id::key_f6 ] = button_states[ ( i32 )e_input_id::key_f6 ];
#endif

		update_button_state( e_input_id::key_pause, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PAUSE ) );

#ifndef _FINAL_RELEASE
		update_button_state( e_input_id::mouse_button_middle, glfwGetMouseButton( g_engine->window.glfw_window, GLFW_MOUSE_BUTTON_MIDDLE ) );
		update_button_state( e_input_id::key_f5, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F5 ) );
		update_button_state( e_input_id::key_f6, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F6 ) );
#endif
	}
	else
	{
		button_states_last_frame = button_states;

		update_button_state( e_input_id::key_backspace, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_BACKSPACE ) );
		update_button_state( e_input_id::key_shift_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_SHIFT ) );
		update_button_state( e_input_id::key_shift_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_SHIFT ) );
		update_button_state( e_input_id::key_control_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_CONTROL ) );
		update_button_state( e_input_id::key_control_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_CONTROL ) );
		update_button_state( e_input_id::key_alt_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_ALT ) );
		update_button_state( e_input_id::key_alt_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_ALT ) );
		update_button_state( e_input_id::mouse_button_left, glfwGetMouseButton( g_engine->window.glfw_window, GLFW_MOUSE_BUTTON_LEFT ) );
		update_button_state( e_input_id::mouse_button_middle, glfwGetMouseButton( g_engine->window.glfw_window, GLFW_MOUSE_BUTTON_MIDDLE ) );
		update_button_state( e_input_id::mouse_button_right, glfwGetMouseButton( g_engine->window.glfw_window, GLFW_MOUSE_BUTTON_RIGHT ) );
		update_button_state( e_input_id::key_esc, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_ESCAPE ) );
		update_button_state( e_input_id::key_pause, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PAUSE ) );
		update_button_state( e_input_id::key_left_bracket, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT_BRACKET ) );
		update_button_state( e_input_id::key_right_bracket, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT_BRACKET ) );
		update_button_state( e_input_id::key_enter, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_ENTER ) );
		update_button_state( e_input_id::key_space, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_SPACE ) );
		update_button_state( e_input_id::key_f1, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F1 ) );
		update_button_state( e_input_id::key_f2, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F2 ) );
		update_button_state( e_input_id::key_f3, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F3 ) );
		update_button_state( e_input_id::key_f4, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F4 ) );
		update_button_state( e_input_id::key_f5, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F5 ) );
		update_button_state( e_input_id::key_f6, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F6 ) );
		update_button_state( e_input_id::key_f7, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F7 ) );
		update_button_state( e_input_id::key_f8, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F8 ) );
		update_button_state( e_input_id::key_f9, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F9 ) );
		update_button_state( e_input_id::key_f10, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F10 ) );
		update_button_state( e_input_id::key_f11, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F11 ) );
		update_button_state( e_input_id::key_f12, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F12 ) );
		update_button_state( e_input_id::key_a, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_A ) );
		update_button_state( e_input_id::key_b, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_B ) );
		update_button_state( e_input_id::key_c, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_C ) );
		update_button_state( e_input_id::key_d, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_D ) );
		update_button_state( e_input_id::key_e, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_E ) );
		update_button_state( e_input_id::key_f, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_F ) );
		update_button_state( e_input_id::key_g, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_G ) );
		update_button_state( e_input_id::key_h, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_H ) );
		update_button_state( e_input_id::key_i, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_I ) );
		update_button_state( e_input_id::key_j, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_I ) );
		update_button_state( e_input_id::key_k, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_K ) );
		update_button_state( e_input_id::key_l, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_L ) );
		update_button_state( e_input_id::key_m, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_M ) );
		update_button_state( e_input_id::key_n, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_N ) );
		update_button_state( e_input_id::key_o, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_O ) );
		update_button_state( e_input_id::key_p, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_P ) );
		update_button_state( e_input_id::key_q, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_Q ) );
		update_button_state( e_input_id::key_r, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_R ) );
		update_button_state( e_input_id::key_s, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_S ) );
		update_button_state( e_input_id::key_t, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_T ) );
		update_button_state( e_input_id::key_u, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_U ) );
		update_button_state( e_input_id::key_v, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_V ) );
		update_button_state( e_input_id::key_w, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_W ) );
		update_button_state( e_input_id::key_x, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_X ) );
		update_button_state( e_input_id::key_y, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_Y ) );
		update_button_state( e_input_id::key_z, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_Z ) );
		update_button_state( e_input_id::key_0, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_0 ) );
		update_button_state( e_input_id::key_1, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_1 ) );
		update_button_state( e_input_id::key_2, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_2 ) );
		update_button_state( e_input_id::key_3, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_3 ) );
		update_button_state( e_input_id::key_4, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_4 ) );
		update_button_state( e_input_id::key_5, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_5 ) );
		update_button_state( e_input_id::key_6, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_6 ) );
		update_button_state( e_input_id::key_7, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_7 ) );
		update_button_state( e_input_id::key_8, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_8 ) );
		update_button_state( e_input_id::key_9, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_9 ) );
		update_button_state( e_input_id::key_up, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_UP ) );
		update_button_state( e_input_id::key_down, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_DOWN ) );
		update_button_state( e_input_id::key_left, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_LEFT ) );
		update_button_state( e_input_id::key_right, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_RIGHT ) );
		update_button_state( e_input_id::key_period, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PERIOD ) );
		update_button_state( e_input_id::key_comma, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_COMMA ) );
		update_button_state( e_input_id::key_slash, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_SLASH ) );
		update_button_state( e_input_id::key_tilde, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_GRAVE_ACCENT ) );
		update_button_state( e_input_id::key_insert, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_INSERT ) );
		update_button_state( e_input_id::key_delete, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_DELETE ) );
		update_button_state( e_input_id::key_home, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_HOME ) );
		update_button_state( e_input_id::key_end, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_END ) );
		update_button_state( e_input_id::key_page_up, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PAGE_UP ) );
		update_button_state( e_input_id::key_page_down, glfwGetKey( g_engine->window.glfw_window, GLFW_KEY_PAGE_DOWN ) );

		// update game controller states

		if( gamepad.has_value() )
		{
			gamepad->update();

			// update state information and queue events

			gamepad->update_button_state( e_input_id::gamepad_button_a, XINPUT_GAMEPAD_A );
			gamepad->update_button_state( e_input_id::gamepad_button_b, XINPUT_GAMEPAD_B );
			gamepad->update_button_state( e_input_id::gamepad_button_x, XINPUT_GAMEPAD_X );
			gamepad->update_button_state( e_input_id::gamepad_button_y, XINPUT_GAMEPAD_Y );
			gamepad->update_button_state( e_input_id::gamepad_button_dpad_left, XINPUT_GAMEPAD_DPAD_LEFT );
			gamepad->update_button_state( e_input_id::gamepad_button_dpad_right, XINPUT_GAMEPAD_DPAD_RIGHT );
			gamepad->update_button_state( e_input_id::gamepad_button_dpad_up, XINPUT_GAMEPAD_DPAD_UP );
			gamepad->update_button_state( e_input_id::gamepad_button_dpad_down, XINPUT_GAMEPAD_DPAD_DOWN );
			gamepad->update_button_state( e_input_id::gamepad_button_left_thumb, XINPUT_GAMEPAD_LEFT_THUMB );
			gamepad->update_button_state( e_input_id::gamepad_button_right_thumb, XINPUT_GAMEPAD_RIGHT_THUMB );
			gamepad->update_button_state( e_input_id::gamepad_button_left_shoulder, XINPUT_GAMEPAD_LEFT_SHOULDER );
			gamepad->update_button_state( e_input_id::gamepad_button_right_shoulder, XINPUT_GAMEPAD_RIGHT_SHOULDER );
			gamepad->update_button_state( e_input_id::gamepad_button_start, XINPUT_GAMEPAD_START );
			gamepad->update_button_state( e_input_id::gamepad_button_back, XINPUT_GAMEPAD_BACK );
		}
	}
}

void Input_Mgr::queue_motion()
{
	// mouse motion
	//
	// NOTE: mouse motion deltas are sent once per update, not for each message
	// from the OS. this is done to prevent tons of little messages from
	// clogging up the works.

	// window

	if( !fequals( mouse_move_delta.x + mouse_move_delta.y, 0.f ) )
	{
		Input_Event evt;
		evt.event_id = e_event_id::input_motion;
		evt.input_id = e_input_id::mouse;
		evt.delta = mouse_move_delta;

		event_queue.push_back( evt );

		mouse_move_delta = Vec2::zero;
	}

	// mouse wheel

	if( !fequals( mouse_wheel_delta.x + mouse_wheel_delta.y, 0.f ) )
	{
		Input_Event evt;
		evt.event_id = e_event_id::input_motion;
		evt.input_id = e_input_id::mouse_wheel;
		evt.delta = mouse_wheel_delta;

		event_queue.push_back( evt );

		mouse_wheel_delta = Vec2::zero;
	}

	// game controller

	if( gamepad )
	{
		gamepad->update();

		update_axis_delta( e_input_id::gamepad_left_stick );
		update_axis_delta( e_input_id::gamepad_right_stick );
		update_axis_delta( e_input_id::gamepad_left_trigger );
		update_axis_delta( e_input_id::gamepad_right_trigger );
	}
}

void Input_Mgr::update_axis_delta( e_input_id input_id ) const
{
	Vec2 delta = g_engine->input_mgr.get_axis_state( input_id );

	if( !delta.is_zero() )
	{
		Input_Event evt;
		evt.event_id = e_event_id::input_motion;
		evt.input_id = input_id;
		evt.delta = delta;

		g_engine->input_mgr.event_queue.push_back( evt );
	}
}

void Input_Mgr::dispatch_event_queue()
{
	// send all queued messages down the virtual function call chain,
	// and then empty the queue.

	for( auto& evt : event_queue )
	{
		switch( evt.event_id )
		{
			case e_event_id::input_pressed:
			{
				auto ret = g_engine->scene_mgr.on_input( &evt );

				if( ret == e_on_input_result::handled )
				{
					break;
				}

				if( g_engine->on_input( &evt ) )
				{
					break;
				}

				if( ret == e_on_input_result::handled_with_blocking )
				{
					break;
				}

				break;
			}

			case e_event_id::input_held:
			{
				auto ret = g_engine->scene_mgr.on_input( &evt );

				if( ret == e_on_input_result::handled )
				{
					break;
				}

				if( g_engine->on_input( &evt ) )
				{
					break;
				}

				if( ret == e_on_input_result::handled_with_blocking )
				{
					break;
				}

				break;
			}

			case e_event_id::input_released:
			{
				auto ret = g_engine->scene_mgr.on_input( &evt );

				if( ret == e_on_input_result::handled )
				{
					break;
				}

				if( g_engine->on_input( &evt ) )
				{
					break;
				}

				if( ret == e_on_input_result::handled_with_blocking )
				{
					break;
				}

				break;
			}

			case e_event_id::input_motion:
			{
				auto ret = g_engine->scene_mgr.on_input( &evt );

				if( ret == e_on_input_result::handled )
				{
					break;
				}

				if( g_engine->on_input( &evt ) )
				{
					break;
				}

				if( ret == e_on_input_result::handled_with_blocking )
				{
					break;
				}

				break;
			}

			case e_event_id::input_key:
			{
				auto ret = g_engine->scene_mgr.on_input( &evt );

				if( ret == e_on_input_result::handled )
				{
					break;
				}

				if( g_engine->on_input( &evt ) )
				{
					break;
				}

				if( ret == e_on_input_result::handled_with_blocking )
				{
					break;
				}

				break;
			}
		}
	}

	event_queue.clear();
}

void Input_Mgr::update_button_state( e_input_id input_id, i32 glfw_state )
{
	button_states[ (i32)input_id ] = glfw_state;

	switch( get_button_state( input_id ) )
	{
		case e_button_state::pressed:
		{
			g_engine->input_mgr.event_queue.emplace_back( e_event_id::input_pressed, input_id );

			timer_repeat.restart();
			break;
		}

		case e_button_state::released:
		{
			g_engine->input_mgr.event_queue.emplace_back( e_event_id::input_released, input_id );
			break;
		}

		case e_button_state::held:
		{
			if( timer_repeat.get_elapsed() )
			{
				g_engine->input_mgr.event_queue.emplace_back( e_event_id::input_held, input_id );

				timer_repeat.restart();
				g_ui->caret_blink_tween.restart();
			}
			break;
		}
	}
}

void Input_Mgr::play_rumble( e_rumble_effect effect )
{
	if( !gamepad or !gamepad->is_being_used )
	{
		return;
	}

	gamepad->play_rumble( effect );
}

void Input_Mgr::refresh_connected_gamepads()
{
	if( gamepad.has_value() )
	{
		gamepad->play_rumble( e_rumble_effect::none );
	}

	gamepad = std::nullopt;

	// look for an attached xbox controller. the first valid one
	// we find is the one we use for player input.

	XINPUT_STATE state;
	ZeroMemory( &state, sizeof( XINPUT_STATE ) );

	i32 xinput_player_id = -1;
	for( auto pn = 0 ; pn < XUSER_MAX_COUNT and xinput_player_id == -1 ; ++pn )
	{
		if( XInputGetState( pn, &state ) == ERROR_SUCCESS )
		{
			xinput_player_id = pn;

			log( "Using controller : player_id : {}", xinput_player_id );
			gamepad = Game_Controller( xinput_player_id );
			break;
		}
	}
}

bool Input_Mgr::is_button_down( e_input_id input_id )
{
	e_button_state bs = get_button_state( input_id );
	return ( bs == e_button_state::pressed or bs == e_button_state::held );
}

bool Input_Mgr::is_button_held( e_input_id input_id )
{
	e_button_state bs = get_button_state( input_id );
	return ( bs == e_button_state::held );
}

bool Input_Mgr::is_shift_down()
{
	e_button_state bs_left = get_button_state( e_input_id::key_shift_left );
	e_button_state bs_right = get_button_state( e_input_id::key_shift_right );

	return (
		bs_left == e_button_state::pressed
		or bs_left == e_button_state::held
		or bs_right == e_button_state::pressed
		or bs_right == e_button_state::held
		);
}

bool Input_Mgr::is_control_down()
{
	e_button_state bs_left = get_button_state( e_input_id::key_control_left );
	e_button_state bs_right = get_button_state( e_input_id::key_control_right );

	return (
		bs_left == e_button_state::pressed
		or bs_left == e_button_state::held
		or bs_right == e_button_state::pressed
		or bs_right == e_button_state::held
		);
}

bool Input_Mgr::is_alt_down()
{
	e_button_state bs_left = get_button_state( e_input_id::key_alt_left );
	e_button_state bs_right = get_button_state( e_input_id::key_alt_right );

	return (
		bs_left == e_button_state::pressed
		or bs_left == e_button_state::held
		or bs_right == e_button_state::pressed
		or bs_right == e_button_state::held
		);
}

// determines the state of a button by comparing it's current state with the
// previous frame.

e_button_state Input_Mgr::get_button_state( e_input_id input_id )
{
	e_button_state bs = e_button_state::up;

	bool state = button_states[ (i32)input_id ];
	bool state_last_frame = button_states_last_frame[ (i32)input_id ];

	if( state and state_last_frame )
	{
		bs = e_button_state::held;
	}
	else if( state and !state_last_frame )
	{
		bs = e_button_state::pressed;
	}
	else if( !state and state_last_frame )
	{
		bs = e_button_state::released;
	}

	return bs;
}

// returns a value between -1.0/+1.0 for the requested axis.
//
// these values are updated once per frame.

Vec2 Input_Mgr::get_axis_state( e_input_id input_id, bool use_dead_zone )
{
	if( !gamepad )
	{
		return Vec2::zero;
	}

	static f32 gamepad_dead_zone = war::Text_Parser::float_from_str( g_engine->config_vars.find_value_or( "gamepad_dead_zone", "0.25" ) );
	Vec2 value;

	switch( input_id )
	{
		case e_input_id::gamepad_left_stick:
		{
			value.x = glm::max( -1.f, (f32)gamepad->xinput_state.Gamepad.sThumbLX / 32767.f );
			value.y = glm::max( -1.f, (f32)gamepad->xinput_state.Gamepad.sThumbLY / 32767.f ) * -1.f;

			if( use_dead_zone )
			{
				auto dist = glm::abs( value.get_size() );
				if( dist < gamepad_dead_zone )
				{
					value.x = value.y = 0.f;
				}
			}
			break;
		}

		case e_input_id::gamepad_right_stick:
		{
			value.x = glm::max( -1.f, (f32)gamepad->xinput_state.Gamepad.sThumbRX / 32767.f );
			value.y = glm::max( -1.f, (f32)gamepad->xinput_state.Gamepad.sThumbRY / 32767.f ) * -1.f;

			if( use_dead_zone )
			{
				auto dist = glm::abs( value.get_size() );
				if( dist < gamepad_dead_zone )
				{
					value.x = value.y = 0.f;
				}
			}
			break;
		}

		case e_input_id::gamepad_left_trigger:
		{
			value.x = value.y = gamepad->xinput_state.Gamepad.bLeftTrigger / 255.f;
			break;
		}

		case e_input_id::gamepad_right_trigger:
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
