#pragma once

struct w_input_event
{
	// which event this is. required when processing events through the queue.
	e_event_id event_id = e_event_id::invalid;
		
	// the key, mouse button, or controller button generating the event
	e_input_id input_id = e_input_id::invalid;

	// any modifier keys that were pressed at the time of the input event
	bool alt_down = false;
	bool shift_down = false;
	bool ctrl_down = false;

	struct  
	{
		w_vec2 pos = w_vec2( 0, 0 );
		w_vec2 delta = w_vec2( 0, 0 );
	} mouse;
};

// ----------------------------------------------------------------------------

struct w_game_controller
{
	// this can change if a controller gets unplugged during gameplay or after the game starts
	bool is_connected = false;

	int idx = -1;
	float rumble_time_remaining_ms = 0;
	std::unique_ptr<w_timer> timer_repeat = nullptr;

	/*
		 set to true the first time a button is pressed on the controller
	
		 this is how the engine knows to play rumble effects on that controller or not
	*/
	bool is_being_used = false;

	XINPUT_STATE xinput_state;

	w_game_controller( int idx );

	void update_button_state( e_input_id input_id, int xinput_button_bit );
	void update();
	void play_rumble( int intensity, int ms );
};

// ----------------------------------------------------------------------------

struct w_input_mgr : i_speaker
{
	std::map<int, e_input_id> glfw_codes;
	std::vector<w_input_event> event_queue;

	// tracks the mouse movement delta since the last call to Update()
	w_vec2 mouse_move_delta = w_vec2( 0, 0 );

	// where the mouse currently sits in the virtual window
	w_vec2 mouse_vwindow_pos = w_vec2( 0, 0 );
	c2Circle c2_mouse_vpos;

	/*
		there is one entry in this vector for each joystick currently
		present. this list can change over the course of the game
		running as joysticks are plugged in or unplugged.
	*/
	std::unique_ptr<w_game_controller> game_controller = nullptr;

	/*
		this is set in real time, as input is collected. we need to
		know this so we don't rumble a controller that isn't
		actively being used to play the game.
	*/
	bool game_controller_being_used = false;

	// 0 = unpressed
	// 1 = pressed
	// 2 = held
	// 3 = released

	std::array<int, (int)e_input_id::max> button_states;

	bool is_button_down( e_input_id input_id );
	w_vec2 axis_value_of( e_input_id button );

	void init();
	void deinit();
	void update();

	void play_rumble( e_rumble_effect effect );
};
