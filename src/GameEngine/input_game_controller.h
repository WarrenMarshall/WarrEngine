#pragma once

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

