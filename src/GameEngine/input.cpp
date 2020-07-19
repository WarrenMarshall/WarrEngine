
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

// ----------------------------------------------------------------------------

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	bool pressed = !( action == GLFW_RELEASE );

	w_input_event evt;
	evt.event_id = pressed ? e_event_id::input_pressed : e_event_id::input_released;
	evt.input_id = engine->input_mgr->glfw_codes[ key ];

	evt.alt_down = ( mods & GLFW_MOD_ALT );
	evt.shift_down = ( mods & GLFW_MOD_SHIFT );
	evt.ctrl_down = ( mods & GLFW_MOD_CONTROL );

	e_button_state& button_state = engine->input_mgr->button_states[ static_cast<int>( evt.input_id ) ];

	if( action == GLFW_PRESS )
	{
		//log_msg( "PRESSED" );
		button_state = e_button_state::pressed;
	}
	else if( action == GLFW_REPEAT )
	{
		//log_msg( "HELD" );
		button_state = e_button_state::held;
	}
	else
	{
		//log_msg( "RELEASED" );
		button_state = e_button_state::released;
	}

	engine->input_mgr->event_queue.emplace_back( std::move( evt ) );
}

void mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
{
	bool pressed = !( action == GLFW_RELEASE );

	w_input_event evt;
	evt.event_id = pressed ? e_event_id::input_pressed : e_event_id::input_released;
	evt.input_id = engine->input_mgr->glfw_codes[ button ];

	evt.alt_down = ( mods & GLFW_MOD_ALT );
	evt.shift_down = ( mods & GLFW_MOD_SHIFT );
	evt.ctrl_down = ( mods & GLFW_MOD_CONTROL );

	e_button_state& button_state = engine->input_mgr->button_states[ static_cast<int>( evt.input_id ) ];

	if( action == GLFW_PRESS )
	{
		//log_msg( "PRESSED" );
		button_state = e_button_state::pressed;
	}
	else
	{
		//log_msg( "RELEASE" );
		button_state = e_button_state::released;
	}

	engine->input_mgr->event_queue.emplace_back( std::move( evt ) );
}

static w_vec2 last_mouse_pos( 0, 0 );

void mouse_motion_callback( GLFWwindow* window, double xpos, double ypos )
{
	engine->input_mgr->mouse_move_delta.x += static_cast<float>( xpos ) - last_mouse_pos.x;
	engine->input_mgr->mouse_move_delta.y += static_cast<float>( ypos ) - last_mouse_pos.y;

	last_mouse_pos = w_vec2( static_cast<float>( xpos ), static_cast<float>( ypos ) );

	// convert the window space window mouse position into a position on the virtual screen.
	float ratio = ( v_window_w / engine->window->viewport_pos_sz.w );
	float vx = static_cast<float>( ( xpos - engine->window->viewport_pos_sz.x ) * ratio );
	float vy = static_cast<float>( ( ypos - engine->window->viewport_pos_sz.y ) * ratio );

	// only update the position if the mouse is moving over the virtual window itself
	if( vx >= 0 && vx <= v_window_w && vy >= 0 && vy <= v_window_h )
	{
		engine->input_mgr->mouse_vwindow_pos.x = vx;
		engine->input_mgr->mouse_vwindow_pos.y = vy;

		engine->input_mgr->c2_mouse_vpos.p = { engine->input_mgr->mouse_vwindow_pos.x, engine->input_mgr->mouse_vwindow_pos.y };
		engine->input_mgr->c2_mouse_vpos.r = 2.0f;	// gives the mouse a little play with the clicking zone
	}
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
		log_msg( "%s : using controller : %d", __FUNCTION__, 0 );
		game_controller = std::make_unique<w_game_controller>( 0 );
	}

	// callbacks so we can collect user input
	glfwSetKeyCallback( engine->window->window, key_callback );
	glfwSetMouseButtonCallback( engine->window->window, mouse_button_callback );
	glfwSetCursorPosCallback( engine->window->window, mouse_motion_callback );

	for( int x = 0; x < static_cast<int>( e_input_id::max ); ++x )
	{
		button_states[x] = e_button_state::released;
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
		evt.event_id = e_event_id::input_motion;
		evt.input_id = e_input_id::mouse;
		evt.mouse.delta = mouse_move_delta;

		event_queue.emplace_back( std::move( evt ) );

		mouse_move_delta = w_vec2( 0.0f, 0.0f );
	}

	// send every accumulated input message to anyone listening

	for( auto& evt : event_queue )
	{
		send_event_to_listeners( evt.event_id, &evt );
	}

	event_queue.clear();

	// update game controller states

	if( game_controller )
	{
		game_controller->update();
	}
}

void w_input::play_rumble( e_rumble_effect effect )
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

bool w_input::is_button_down( e_input_id input_id )
{
	return button_states[static_cast<int>( input_id )] != e_button_state::released;
}

/*
	returns a value between -1.0/+1.0 for the requested axis.

	these values are updated once per frame.
*/

static float controller_dead_zone = 0.15f;
w_vec2 w_input::axis_value_of( e_input_id input_id )
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
