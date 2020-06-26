
#include "master_pch.h"
#include "master_header.h"

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	switch( action )
	{
		case GLFW_PRESS:
		{
			w_input_event_data data;
			data.input_id = engine->input_mgr->glfw_codes[key];
			data.mods = mods;

			engine->input_mgr->button_states[static_cast<int>( data.input_id )] = true;
			engine->input_mgr->event_input_pressed( e_event_id::input_pressed, data );
		}
		break;

		case GLFW_RELEASE:
		{
			w_input_event_data data;
			data.input_id = engine->input_mgr->glfw_codes[key];
			data.mods = mods;

			engine->input_mgr->button_states[static_cast<int>( data.input_id )] = false;
			engine->input_mgr->event_input_released( e_event_id::input_released, data );
		}
		break;
	}
}

void mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
{
	switch( action )
	{
		case GLFW_PRESS:
		{
			w_input_event_data data;
			data.input_id = engine->input_mgr->glfw_codes[button];
			data.mods = mods;

			engine->input_mgr->button_states[static_cast<int>( data.input_id )] = true;
			engine->input_mgr->event_input_pressed( e_event_id::input_pressed, data );
		}
		break;

		case GLFW_RELEASE:
		{
			w_input_event_data data;
			data.input_id = engine->input_mgr->glfw_codes[button];
			data.mods = mods;

			engine->input_mgr->button_states[static_cast<int>( data.input_id )] = false;
			engine->input_mgr->event_input_released( e_event_id::input_released, data );
		}
		break;
	}
}

static w_vec2 last_mouse_pos( 0, 0 );

void mouse_motion_callback( GLFWwindow* window, double xpos, double ypos )
{
	w_input_event_data data;

	// raw mouse delta

	data.xdelta = static_cast<float>( xpos ) - last_mouse_pos.x;
	data.ydelta = static_cast<float>( ypos ) - last_mouse_pos.y;

	last_mouse_pos = w_vec2( static_cast<float>( xpos ), static_cast<float>( ypos ) );

	// convert the window space window mouse position into a position on the virtual screen.
	float ratio = ( v_window_w / engine->window->viewport_pos_sz.w );
	float vx = static_cast<float>( ( xpos - engine->window->viewport_pos_sz.x ) * ratio );
	float vy = static_cast<float>( ( ypos - engine->window->viewport_pos_sz.y ) * ratio );

	vx = w_clamp( vx, 0.0f, v_window_w );
	vy = w_clamp( vy, 0.0f, v_window_h );

	data.xpos = vx;
	data.ypos = vy;

	engine->input_mgr->event_input_motion( e_event_id::input_motion, data );
}

// ----------------------------------------------------------------------------

w_input_event_data::w_input_event_data()
{
	input_id = e_input_id::invalid;
	mods = -1;
	xpos = ypos = 0.0f;
	xdelta = ydelta = 0.0f;
}

// ----------------------------------------------------------------------------

w_input_event::w_input_event()
{
	dead = true;
	event_id = e_event_id::invalid;
}

// ----------------------------------------------------------------------------

w_game_controller::w_game_controller( int idx )
	: idx( idx )
{
}

w_game_controller::~w_game_controller()
{
}

void w_game_controller::update_button_state( e_input_id input_id, int xinput_button_bit )
{
	bool last_state = engine->input_mgr->button_states[static_cast<int>( input_id )];
	bool current_state = (xinput_state.Gamepad.wButtons & xinput_button_bit) > 0;
	engine->input_mgr->button_states[static_cast<int>( input_id )] = current_state;

	if( !last_state && current_state )
	{
		w_input_event_data data;
		data.input_id = input_id;

		is_being_used = true;
		engine->input_mgr->event_input_pressed( e_event_id::input_pressed, data );
	}
	else if( last_state && !current_state )
	{
		w_input_event_data data;
		data.input_id = input_id;

		engine->input_mgr->event_input_released( e_event_id::input_released, data );
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

// ----------------------------------------------------------------------------

/*
	design notes

	- buttons being pressed and released are sent as listener events
	- buttons being held can be queried as needed
	- motion can be queried (and accumulated +/- until queried?)
*/

void w_input_mgr::init()
{
	// look for an attached xbox controller

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
 
	if( XInputGetState( 0, &state ) == ERROR_SUCCESS )
	{
		// controller is connected on port 1
		log_msg( "%s : using controller : %d", __FUNCTION__, 0 );
		game_controller = std::make_unique<w_game_controller>( 0 );
	}

	// set up states and queues

	event_queue = std::make_unique<std::vector<w_input_event>>( w_input_mgr::event_queue_max );
	event_queue_idx = 0;

	// callbacks so we can collect user input
	glfwSetKeyCallback( engine->window->window, key_callback );
	glfwSetMouseButtonCallback( engine->window->window, mouse_button_callback );
	glfwSetCursorPosCallback( engine->window->window, mouse_motion_callback );

	for( int x = 0; x < static_cast<int>( e_input_id::max ); ++x )
	{
		button_states[x] = false;;
	}

	glfw_codes.insert( std::make_pair( GLFW_KEY_ESCAPE, e_input_id::keyboard_esc ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_PAUSE, e_input_id::keyboard_pause ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_LEFT_BRACKET, e_input_id::keyboard_left_bracket ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_RIGHT_BRACKET, e_input_id::keyboard_right_bracket ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_ENTER, e_input_id::keyboard_enter ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_SPACE, e_input_id::keyboard_space ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F1, e_input_id::keyboard_f1 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F2, e_input_id::keyboard_f2 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F3, e_input_id::keyboard_f3 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F4, e_input_id::keyboard_f4 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F5, e_input_id::keyboard_f5 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F6, e_input_id::keyboard_f6 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F7, e_input_id::keyboard_f7 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F8, e_input_id::keyboard_f8 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F9, e_input_id::keyboard_f9 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F10, e_input_id::keyboard_f10 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F11, e_input_id::keyboard_f11 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F12, e_input_id::keyboard_f12 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_A, e_input_id::keyboard_a ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_B, e_input_id::keyboard_b ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_C, e_input_id::keyboard_c ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_D, e_input_id::keyboard_d ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_E, e_input_id::keyboard_e ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_F, e_input_id::keyboard_f ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_G, e_input_id::keyboard_g ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_H, e_input_id::keyboard_h ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_I, e_input_id::keyboard_i ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_I, e_input_id::keyboard_j ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_K, e_input_id::keyboard_k ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_L, e_input_id::keyboard_l ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_M, e_input_id::keyboard_m ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_N, e_input_id::keyboard_n ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_O, e_input_id::keyboard_o ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_P, e_input_id::keyboard_p ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_Q, e_input_id::keyboard_q ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_R, e_input_id::keyboard_r ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_S, e_input_id::keyboard_s ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_T, e_input_id::keyboard_t ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_U, e_input_id::keyboard_u ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_V, e_input_id::keyboard_v ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_W, e_input_id::keyboard_w ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_X, e_input_id::keyboard_x ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_Y, e_input_id::keyboard_y ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_Z, e_input_id::keyboard_z ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_0, e_input_id::keyboard_0 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_1, e_input_id::keyboard_1 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_2, e_input_id::keyboard_2 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_3, e_input_id::keyboard_3 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_4, e_input_id::keyboard_4 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_5, e_input_id::keyboard_5 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_6, e_input_id::keyboard_6 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_7, e_input_id::keyboard_7 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_8, e_input_id::keyboard_8 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_9, e_input_id::keyboard_9 ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_UP, e_input_id::keyboard_up ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_DOWN, e_input_id::keyboard_down ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_LEFT, e_input_id::keyboard_left ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_RIGHT, e_input_id::keyboard_right ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_PERIOD, e_input_id::keyboard_period ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_COMMA, e_input_id::keyboard_comma ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_SLASH, e_input_id::keyboard_slash ) );
	glfw_codes.insert( std::make_pair( GLFW_KEY_GRAVE_ACCENT, e_input_id::keyboard_tilde ) );
	glfw_codes.insert( std::make_pair( GLFW_MOUSE_BUTTON_LEFT, e_input_id::mouse_button_left ) );
	glfw_codes.insert( std::make_pair( GLFW_MOUSE_BUTTON_MIDDLE, e_input_id::mouse_button_middle ) );
	glfw_codes.insert( std::make_pair( GLFW_MOUSE_BUTTON_RIGHT, e_input_id::mouse_button_right ) );
	//glfw_codes.insert( std::make_pair( GLFW_GAMEPAD_BUTTON_A, e_input_id::controller_button_a) );
	//glfw_codes.insert( std::make_pair( GLFW_GAMEPAD_BUTTON_B, e_input_id::controller_button_b) );
	//glfw_codes.insert( std::make_pair( GLFW_GAMEPAD_BUTTON_X, e_input_id::controller_button_x) );
	//glfw_codes.insert( std::make_pair( GLFW_GAMEPAD_BUTTON_Y, e_input_id::controller_button_y) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_DPAD_LEFT, e_input_id::controller_button_dpad_left) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_DPAD_RIGHT, e_input_id::controller_button_dpad_right) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_DPAD_UP, e_input_id::controller_button_dpad_up) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_DPAD_DOWN, e_input_id::controller_button_dpad_down) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_START, e_input_id::controller_button_start) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_BACK, e_input_id::controller_button_back) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_LEFT_THUMB, e_input_id::controller_button_left_thumb) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_RIGHT_THUMB, e_input_id::controller_button_right_thumb) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_LEFT_SHOULDER, e_input_id::controller_button_left_shoulder ) );
	//glfw_codes.insert( std::make_pair( XINPUT_GAMEPAD_RIGHT_SHOULDER, e_input_id::controller_button_right_shoulder ) );
}

void w_input_mgr::deinit()
{
	game_controller = nullptr;
}

void w_input_mgr::update()
{
	// send the queued up events to anyone listening

	for( auto& evt : *event_queue )
	{
		if( evt.dead == false )
		{
			send_event_to_listeners( evt.event_id, &evt );
			evt.dead = true;
		}
	}

	// update game controller states

	if( game_controller )
	{
		game_controller->update();
	}

	// NOTE : mouse and keyboard states are updated through the glfw callback functions
}

void w_input_mgr::play_rumble( e_rumble_effect effect )
{
	if( !game_controller || !game_controller->is_being_used )
	{
		return;
	}

	int rumble_max = 65535;
	int intensity = 65535;
	int duration_ms = 600;

	switch( effect )
	{
		case e_rumble_effect::medium:
		{
			intensity = static_cast<int>( rumble_max * 0.75f );
			duration_ms = 400;
		}
		break;

		case e_rumble_effect::small:
		{
			intensity = static_cast<int>( rumble_max * 0.5f );
			duration_ms = 300;
		}
		break;

		case e_rumble_effect::tiny:
		{
			intensity = static_cast<int>( rumble_max * 0.35f );
			duration_ms = 200;
		}
		break;
	}

	game_controller->play_rumble( intensity, duration_ms );
}

bool w_input_mgr::is_button_down( e_input_id input_id )
{
	return button_states[static_cast<int>( input_id )] == GLFW_PRESS;
}

/*
	returns a value between -1.0/+1.0 for the requested axis.

	these values are updated once per frame.
*/

static float controller_dead_zone = 0.15f;
w_vec2 w_input_mgr::axis_value_of( e_input_id input_id )
{
	if( !game_controller || !game_controller->is_being_used )
	{
		return w_vec2( 0, 0 );
	}

	game_controller->update();
	w_vec2 value;

	switch( input_id )
	{
		case e_input_id::controller_left_stick:
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

		case e_input_id::controller_right_stick:
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

		case e_input_id::controller_left_trigger:
		{
			value.x = game_controller->xinput_state.Gamepad.bLeftTrigger / 255.0f;
		}
		break;

		case e_input_id::controller_right_trigger:
		{
			value.x = game_controller->xinput_state.Gamepad.bRightTrigger / 255.0f;
		}
		break;

		default:
		{
			log_error( "%s : unknown axis", __FUNCTION__ );
		}
		break;
	}

	// the value being returned should be normalized
	return value;
}

// a button has been pushed down

void w_input_mgr::event_input_pressed( e_event_id event_id, w_input_event_data data )
{
	w_input_event* evt = get_next_event_slot();
	evt->event_id = event_id;
	evt->data = data;
}

// a button has been released

void w_input_mgr::event_input_released( e_event_id event_id, w_input_event_data data )
{
	w_input_event* evt = get_next_event_slot();
	evt->event_id = event_id;
	evt->data = data;
}

// some sort of movement has happened

void w_input_mgr::event_input_motion( e_event_id event_id, w_input_event_data data )
{
	switch( event_id )
	{
		case e_event_id::input_motion:
		{
			w_input_event* evt = get_next_event_slot();
			evt->event_id = event_id;
			evt->data = data;
		}
		break;

		default:
		{
			log_error( "%s : unsupported event id", __FUNCTION__ );
		}
		break;
	}
}

w_input_event* w_input_mgr::get_next_event_slot()
{
	event_queue_idx = ( event_queue_idx + 1 ) % w_input_mgr::event_queue_max;

	w_input_event* evt = &( *event_queue )[event_queue_idx];
	evt->event_id = e_event_id::invalid;

	evt->data.input_id = e_input_id::invalid;
	evt->data.mods = -1;

	evt->dead = false;

	return evt;
}

void w_input_mgr::set_mouse_mode( e_mouse_mode mode )
{
	switch( mode )
	{
		case e_mouse_mode::normal:
		{
			glfwSetInputMode( engine->window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
		}
		break;

		case e_mouse_mode::hidden:
		{
			glfwSetInputMode( engine->window->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
		}
		break;

		case e_mouse_mode::locked:
		{
			glfwSetInputMode( engine->window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
		}
		break;
	}
}
