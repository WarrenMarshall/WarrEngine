
namespace war
{

struct Input_Event
{
	Input_Event();

	// which event this is. required when processing events through the queue.
	e_event_id_t event_id = e_event_id::invalid;

	// the key, mouse button, or controller button generating the event
	e_input_id_t input_id = e_input_id::invalid;

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
};

static_assert( sizeof( Input_Event ) <= 64 );

// ----------------------------------------------------------------------------

struct Input_Mgr
{
	// holds onto generated input events until the update
	// function can send them to anyone listening
	std::vector<Input_Event> event_queue;
	std::unique_ptr<Timer> timer_repeat = nullptr;

	// tracks the mouse movement delta since the last call to Update()
	Vec2 mouse_move_delta = Vec2::zero;
	Vec2 mouse_wheel_delta = Vec2::zero;

	// where the mouse currently sits in the window
	Vec2 mouse_window_pos = Vec2::zero;

	std::unique_ptr<Game_Controller> gamepad = nullptr;

	std::array<bool, e_input_id::max> button_states = {};
	std::array<bool, e_input_id::max> button_states_last_frame = {};

	[[nodiscard]] bool is_button_down( e_input_id_t input_id );
	[[nodiscard]] bool is_button_held( e_input_id_t input_id );
	[[nodiscard]] bool is_shift_down();
	[[nodiscard]] bool is_control_down();
	[[nodiscard]] bool is_alt_down();

	[[nodiscard]] e_button_state_t get_button_state( e_input_id_t input_id );
	[[nodiscard]] Vec2 get_axis_state( e_input_id_t input_id, bool use_dead_zone = true );

	void init();
	void deinit();
	void queue_presses();
	void queue_motion();
	void update_axis_delta( e_input_id_t input_id ) const;
	void dispatch_event_queue();

	void update_button_state( e_input_id_t input_id, int32_t glfw_state );
	void play_rumble( e_rumble_effect_t effect ) const;
	void refresh_connected_gamepads();
};

}
