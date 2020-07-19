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

struct w_input : i_speaker
{
	// a way to map GLFW input codes to our internal codes
	std::map<int, e_input_id> glfw_codes;

	// holds onto generated input events until the update
	// function can send them to anyone listening
	std::vector<w_input_event> event_queue;

	// tracks the mouse movement delta since the last call to Update()
	w_vec2 mouse_move_delta = w_vec2( 0, 0 );

	// where the mouse currently sits in the virtual window
	w_vec2 mouse_vwindow_pos = w_vec2( 0, 0 );
	//c2Circle c2_mouse_vpos;

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

	std::array<e_button_state, (int)e_input_id::max> button_states;

	bool is_button_down( e_input_id input_id );
	w_vec2 axis_value_of( e_input_id button );

	void init();
	void deinit();
	void update();

	void play_rumble( e_rumble_effect effect );
};
