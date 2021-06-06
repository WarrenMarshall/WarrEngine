#pragma once

struct w_gamepad
{
	std::unique_ptr<w_timer> timer_repeat = nullptr;
	int player_id = -1;
	float rumble_time_remaining_ms = 0;

	XINPUT_STATE xinput_state = { 0 };

	// this can change if a controller gets unplugged during gameplay or after the game starts
	bool is_connected = false;

	// set to true the first time a button is pressed on the controller. this is
	// how the engine knows to play rumble effects on that controller or not

	bool is_being_used = false;

	w_gamepad( int player_id );

	void update_button_state( e_input_id input_id, int xinput_button_bit );
	void update_state();
	void update();
	void play_rumble( e_rumble_effect effect );
	void play_rumble( int intensity, int ms );
};