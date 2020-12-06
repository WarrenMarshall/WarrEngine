#pragma once

struct w_input_event
{
	// which event this is. required when processing events through the queue.
	e_event_id event_id = event_id::invalid;

	// the key, mouse button, or controller button generating the event
	e_input_id input_id = input_id::invalid;

	// a place for various events to store the deltas. used for mouse and controllers.
	w_vec2 delta = w_vec2( 0, 0 );
};

// ----------------------------------------------------------------------------

struct w_input
{
	// holds onto generated input events until the update
	// function can send them to anyone listening
	//const size_t event_queue_max_size = 8;
	std::vector<w_input_event> event_queue;
	std::unique_ptr<w_timer> timer_repeat = nullptr;

	// tracks the mouse movement delta since the last call to Update()
	w_vec2 mouse_move_delta = w_vec2( 0, 0 );

	// where the mouse currently sits in the ...
	w_vec2 mouse_window_pos = w_vec2( 0, 0 );	// window
	w_vec2 mouse_vwindow_pos = w_vec2( 0, 0 );	// virtual window

	std::unique_ptr<w_gamepad> gamepad = nullptr;

	/*
		this is set in real time, as input is collected. we need to
		know this so we don't rumble a controller that isn't
		actively being used to play the game.
	*/
	bool game_controller_being_used = false;

	std::array<bool, input_id::max> button_states = {};
	std::array<bool, input_id::max> button_states_last_frame = {};

	[[nodiscard]] bool is_button_down( e_input_id input_id );
	[[nodiscard]] bool is_shift_down();
	[[nodiscard]] bool is_control_down();
	[[nodiscard]] bool is_alt_down();

	[[nodiscard]] e_button_state get_button_state( e_input_id input_id );
	[[nodiscard]] w_vec2 get_axis_state( e_input_id input_id, bool ignore_dead_zone = false );

	void init();
	void deinit();
	void queue_presses();
	void queue_motion();
	void update();

	void update_button_state( e_input_id input_id, int glfw_state );
	void play_rumble( e_rumble_effect effect );
	void refresh_connected_gamepads();
};
