
namespace war
{

struct input_event
{
	input_event();

	// which event this is. required when processing events through the queue.
	e_event_id event_id = event_id::invalid;

	// the key, mouse button, or controller button generating the event
	e_input_id input_id = input_id::invalid;

	// the char representation of the key that was pressed
	char ch = 0;

	// current mouse position, in window coordinates
	vec2 mouse_pos = vec2::zero;

	// a place for various events to store the deltas. used for mouse and controllers.
	vec2 delta = vec2::zero;

	// modifier keys
	bool shift_down = false;
	bool control_down = false;
	bool alt_down = false;
};

static_assert( sizeof( input_event ) <= 64 );

// ----------------------------------------------------------------------------

struct input_mgr
{
	// holds onto generated input events until the update
	// function can send them to anyone listening
	std::vector<input_event> event_queue;
	std::unique_ptr<timer> timer_repeat = nullptr;

	// tracks the mouse movement delta since the last call to Update()
	vec2 mouse_move_delta = vec2::zero;
	vec2 mouse_wheel_delta = vec2::zero;

	// where the mouse currently sits in the window
	vec2 mouse_window_pos = vec2::zero;

	std::unique_ptr<game_controller> gamepad = nullptr;

	std::array<bool, input_id::max> button_states = {};
	std::array<bool, input_id::max> button_states_last_frame = {};

	[[nodiscard]] bool is_button_down( e_input_id input_id );
	[[nodiscard]] bool is_button_held( e_input_id input_id );
	[[nodiscard]] bool is_shift_down();
	[[nodiscard]] bool is_control_down();
	[[nodiscard]] bool is_alt_down();

	[[nodiscard]] e_button_state get_button_state( e_input_id input_id );
	[[nodiscard]] vec2 get_axis_state( e_input_id input_id, bool use_dead_zone = true );

	void init();
	void deinit();
	void queue_presses();
	void queue_motion();
	void update_axis_delta( e_input_id input_id ) const;
	void dispatch_event_queue();

	void update_button_state( e_input_id input_id, int glfw_state );
	void play_rumble( e_rumble_effect effect ) const;
	void refresh_connected_gamepads();
};

}
