
namespace war
{

struct Input_Event final
{
	Input_Event();
	Input_Event( e_event_id event_id, e_input_id input_id );

	// which event this is. required when processing events through the queue.
	e_event_id event_id = e_event_id::invalid;

	[[nodiscard]] bool is_pressed() const { return event_id == e_event_id::input_pressed; }
	[[nodiscard]] bool is_held() const { return event_id == e_event_id::input_held; }
	[[nodiscard]] bool is_released() const { return event_id == e_event_id::input_released; }
	[[nodiscard]] bool is_key() const { return event_id == e_event_id::input_key; }
	[[nodiscard]] bool is_motion() const { return event_id == e_event_id::input_motion; }

	// the key, mouse button, or controller button generating the event
	e_input_id input_id = e_input_id::invalid;

	// the char representation of the key that was pressed
	char ch = 0;

	// current mouse position, in window coordinates
	Vec2 mouse_pos = Vec2::zero;

	// a place for various events to store the deltas. used for mouse and controllers.
	Vec2 delta = Vec2::zero;

	// modifier keys
	bool shift_down = false;
	bool control_down = false;
	bool alt_down = false;

	[[nodiscard]] bool is_a_gamepad_button() const
	{
		return( input_id == e_input_id::gamepad_button_a
			or input_id == e_input_id::gamepad_button_b
			or input_id == e_input_id::gamepad_button_x
			or input_id == e_input_id::gamepad_button_y
			or input_id == e_input_id::gamepad_button_dpad_down
			or input_id == e_input_id::gamepad_button_dpad_left
			or input_id == e_input_id::gamepad_button_dpad_right
			or input_id == e_input_id::gamepad_button_dpad_up
			or input_id == e_input_id::gamepad_button_left_shoulder
			or input_id == e_input_id::gamepad_button_left_thumb
			or input_id == e_input_id::gamepad_button_right_shoulder
			or input_id == e_input_id::gamepad_button_right_thumb
			or input_id == e_input_id::gamepad_button_start
			or input_id == e_input_id::gamepad_button_back );
	}
};

static_assert( sizeof( Input_Event ) <= 64 );

// ----------------------------------------------------------------------------

struct Input_Mgr final
{
	// holds onto generated input events until the update
	// function can send them to anyone listening
	std::vector<Input_Event> event_queue;
	Timer timer_repeat;

	// tracks the mouse movement delta since the last call to Update()
	Vec2 mouse_move_delta = Vec2::zero;
	Vec2 mouse_wheel_delta = Vec2::zero;

	// where the mouse currently sits in the window
	Vec2 mouse_window_pos = Vec2::zero;

	std::optional<Game_Controller> gamepad = std::nullopt;

	std::array<bool, (i32)e_input_id::max> button_states = {};
	std::array<bool, (i32)e_input_id::max> button_states_last_frame = {};

	[[nodiscard]] bool is_button_down( e_input_id input_id );
	[[nodiscard]] bool is_button_held( e_input_id input_id );
	[[nodiscard]] bool is_shift_down();
	[[nodiscard]] bool is_control_down();
	[[nodiscard]] bool is_alt_down();

	[[nodiscard]] e_button_state get_button_state( e_input_id input_id );
	[[nodiscard]] Vec2 get_axis_state( e_input_id input_id, bool use_dead_zone = true );

	void init();
	void deinit();
	void queue_presses();
	void queue_motion();
	void update_axis_delta( e_input_id input_id ) const;
	void dispatch_event_queue();

	void update_button_state( e_input_id input_id, i32 glfw_state );
	void play_rumble( e_rumble_effect effect );
	void refresh_connected_gamepads();
};

}
