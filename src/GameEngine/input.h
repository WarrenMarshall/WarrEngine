#pragma once

struct GE_API w_input_event
{
	// which event this is. required when processing events through the queue.
	e_event_id event_id = event_id::invalid;
		
	// the key, mouse button, or controller button generating the event
	e_input_id input_id = input_id::invalid;

	struct  
	{
		w_vec2 pos = w_vec2( 0, 0 );
		w_vec2 delta = w_vec2( 0, 0 );
	} mouse;
};

// ----------------------------------------------------------------------------

struct GE_API w_input : i_speaker
{
	// a way to map GLFW input codes to our internal codes
	std::map<int, e_input_id> glfw_codes;

	// holds onto generated input events until the update
	// function can send them to anyone listening
	std::vector<w_input_event> event_queue;
	std::unique_ptr<w_timer> timer_repeat = nullptr;

	// tracks the mouse movement delta since the last call to Update()
	w_vec2 mouse_move_delta = w_vec2( 0, 0 );

	// where the mouse currently sits in the virtual window
	w_vec2 mouse_vwindow_pos = w_vec2( 0, 0 );

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

	std::array<bool, input_id::max> button_states = {};
	std::array<bool, input_id::max> button_states_last_frame = {};

	bool is_button_down( e_input_id input_id );
	bool is_shift_down();
	bool is_control_down();
	bool is_alt_down();
	e_button_state get_button_state( e_input_id input_id );
	w_vec2 axis_value_of( e_input_id button );

	void init();
	void deinit();
	void update();

	void update_button_state( e_input_id input_id, int glfw_state );
	void play_rumble( e_rumble_effect effect );
};
