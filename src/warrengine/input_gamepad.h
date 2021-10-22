
namespace war
{

struct Game_Controller final
{
	Timer timer_repeat;
	i32 player_id = -1;
	f32 rumble_time_remaining_ms = 0.f;

	XINPUT_STATE xinput_state = { 0 };

	// this can change if a controller gets unplugged during gameplay or after the game starts
	bool is_connected = false;

	// set to true the first time a button is pressed on the controller. this is
	// how the engine knows to play rumble effects on that controller or not

	bool is_being_used = false;

	Game_Controller( i32 player_id );

	void update_button_state( e_input_id input_id, i32 xinput_button_bit );
	void update_state();
	void update();
	void play_rumble( e_rumble_effect effect );
	void play_rumble( i32 intensity, i32 ms );
};

}
