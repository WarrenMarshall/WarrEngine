
// ----------------------------------------------------------------------------
// note : needs to match values in data/warrengine/preproc.asset_def

enum struct e_align
{
	fill = 0b00000000,
	left = 0b00000001,
	right = 0b00000010,
	top = 0b00000100,
	bottom = 0b00001000,
	hcenter = 0b00010000,
	vcenter = 0b00100000,
	horizontal = 0b01000000,
	vertical = 0b10000000,
	centered = hcenter | vcenter,
	top_left = top | left,
};

DEFINE_ENUM_FLAG_OPERATORS( e_align );

// ----------------------------------------------------------------------------

enum struct e_timeline_type
{
	invalid,
	f32ype,
	color_type
};

// ----------------------------------------------------------------------------

enum struct e_input_id
{
	invalid,
	gamepad_button_dpad_up,
	gamepad_button_dpad_down,
	gamepad_button_dpad_left,
	gamepad_button_dpad_right,
	gamepad_button_start,
	gamepad_button_back,
	gamepad_button_left_thumb,		// this is a button because you can click the stick in
	gamepad_button_right_thumb,
	gamepad_button_left_shoulder,
	gamepad_button_right_shoulder,
	gamepad_button_a,
	gamepad_button_b,
	gamepad_button_x,
	gamepad_button_y,
	gamepad_left_stick,				// this is for reading the axis movement values
	gamepad_right_stick,
	gamepad_left_trigger,
	gamepad_right_trigger,
	mouse_button_left,
	mouse_button_middle,
	mouse_button_right,
	mouse,
	mouse_wheel,
	key_a, key_b, key_c, key_d, key_e, key_f, key_g,
	key_h, key_i, key_j, key_k, key_l, key_m, key_n,
	key_o, key_p, key_q, key_r, key_s, key_t, key_u,
	key_v, key_w, key_x, key_y, key_z,
	key_0, key_1, key_2, key_3, key_4, key_5, key_6,
	key_7, key_8, key_9,
	key_left_bracket,
	key_right_bracket,
	key_esc,
	key_enter,
	key_space,
	key_f1, key_f2, key_f3, key_f4, key_f5, key_f6,
	key_f7, key_f8, key_f9, key_f10, key_f11, key_f12,
	key_up, key_down, key_left, key_right,
	key_insert, key_delete,
	key_home, key_end,
	key_page_up, key_page_down,
	key_period,
	key_comma,
	key_slash,
	key_at,
	key_tilde,
	key_quote,
	key_double_quote,
	key_backspace,
	key_exclaim,
	key_hash,
	key_percent,
	key_dollar,
	key_ampersand,
	key_and,
	key_asterisk,
	key_left_paren,
	key_right_paren,
	key_less,
	key_equals,
	key_greater,
	key_question,
	key_caret,
	key_underscore,
	key_backslash,
	key_backquote,
	key_pause,
	key_shift_left,
	key_shift_right,
	key_control_left,
	key_control_right,
	key_alt_left,
	key_alt_right,
	max
};

// ----------------------------------------------------------------------------

enum struct e_rumble_effect
{
	none,
	tiny,
	small,
	medium,
	large
};

// ----------------------------------------------------------------------------

enum struct e_life_cycle
{
	invalid,
	alive,
	dying,			// pending delete whenever the object is finished doing everything (components are finished, etc)
	dead			// delete next update iteration, this object isn't needed anymore and has finished all of it's work
};

// ----------------------------------------------------------------------------

enum struct e_event_id
{
	invalid,
	input_pressed,
	input_released,
	input_held,
	input_motion,
	input_key
};

// ----------------------------------------------------------------------------
// note : needs to match values in data/warrengine/preproc.asset_def

enum struct e_tween_type
{
	invalid = -1,
	one_shot,		// value iterates from start to end, once. when complete, it sits at the end forever.
	loop,			// value iterates from start to end and resets to start, looping
	pingpong		// value iterates from start to end and back again, looping
};

// ----------------------------------------------------------------------------

enum struct e_tween_via
{
	invalid = -1,
	linear,
	quadratic,
	quadratic_in,
	quadratic_out,
	cubic,
	cubic_in,
	cubic_out,
	quartic,
	quartic_in,
	quartic_out,
	quintic,
	quintic_in,
	quintic_out,
	sinusoidal,
	sinusoidal_in,
	sinusoidal_out,
	exponential,
	exponential_in,
	exponential_out,
	circular,
	circular_in,
	circular_out,
	bounce,
	bounce_in,
	bounce_out,
	elastic,
	elastic_in,
	elastic_out,
	back,
	back_in,
	back_out,
};

// ----------------------------------------------------------------------------
//	a definition for the source coordinates of a 9-slice image
//
// these are stored row/column:
//
// top_left    top_middle    top_right
// middle_left middle_middle middle_right
// bottom_left bottom_middle bottom_right

enum struct e_slice_def_patch
{
	top_left,
	top_middle,
	top_right,

	middle_left,
	middle_middle,
	middle_right,

	bottom_left,
	bottom_middle,
	bottom_right
};

// ----------------------------------------------------------------------------

enum struct e_opengl_blend
{
	alpha,
	add,
	multiply,
	glow,
};

// ----------------------------------------------------------------------------
// note : needs to match values in data/warrengine/preproc.asset_def

enum struct e_particle_spawn_dir
{
	inherit_from_owner = -1,
	away_from_owner = -2
};

// ----------------------------------------------------------------------------
// os = no custom cursor image, using OS mouse pointer
// custom = custom cursor image, OS mouse pointer is hidden
// locked = no cursor showing at all, mouse is locked to game window

enum struct e_mouse_mode
{
	os,
	custom,
	locked
};

// ----------------------------------------------------------------------------

enum struct e_button_state
{
	invalid,
	pressed,
	held,
	released,
	up,
	max
};

// ----------------------------------------------------------------------------

enum struct e_im_result
{
	none			= 0b00000000,
	hovered			= 0b00000001,
	hot				= 0b00000010,
	left_clicked	= 0b00000100,
};

DEFINE_ENUM_FLAG_OPERATORS( e_im_result );

// ----------------------------------------------------------------------------

enum struct e_follow_flags
{
	none	= 0b00000000,
	x_axis	= 0b00000001,
	y_axis	= 0b00000010,
	angle	= 0b00000100,	// match rotation as well as position

	xy_axis = x_axis | y_axis,
};

DEFINE_ENUM_FLAG_OPERATORS( e_follow_flags );

// ----------------------------------------------------------------------------

enum struct e_ui_control_type
{
	none = -1,
	panel,			// window frame
	caption,		// a caption that sits at the top of a panel
	button,			// button that clicks up/down
	check,			// button that has a toggle state
	divider,		// simple horizontal divider
	spacer,			// invisible dead space, used for aesthetics
	image,			// a textured quad
	label,			// read-only text
	slider,			// horizontal line with a slider thumb
	text,			// single line text entry
	radio,			// one item of several which are mutually exclusive
	progress,		// a bar that marks a %, from 0-100
	list,			// a list of clickable items
	dropdown,		// button that presents a menu of options when clicked
	max,
};

// ----------------------------------------------------------------------------

enum struct e_primitive_shape
{
	rect = 1,
	filled_rect,
	circle,
	filled_circle,
	point
};

// ----------------------------------------------------------------------------

enum struct e_render_prim
{
	quad,
	triangle,
	line,
	point
};

// ----------------------------------------------------------------------------

enum struct e_particle_spawner_type
{
	point = 1,
	box,
	in_circle,
	on_circle,
};

// ----------------------------------------------------------------------------

enum struct e_tiling
{
	clamp,
	repeat,
};

// ----------------------------------------------------------------------------

enum struct e_tile_flags
{
	none = 0b00000000,
	flipped_horizontally = 0b00000001,
	flipped_vertically = 0b00000010,
	flipped_diagonally = 0b00000100,
};

DEFINE_ENUM_FLAG_OPERATORS( e_tile_flags );

// ----------------------------------------------------------------------------

enum struct e_corner
{
	all,
	top_left,
	top_right,
	bottom_right,
	bottom_left,
};

// ----------------------------------------------------------------------------

enum struct e_pal
{
	darkest,
	darker,
	middle,
	lighter,
	lightest,
};

// ----------------------------------------------------------------------------

enum struct e_cut
{
	left,
	right,
	top,
	bottom,
};

// ----------------------------------------------------------------------------

enum struct e_sc_prim_type
{
	aabb,
	circle,
	polygon,
};

// ----------------------------------------------------------------------------

enum struct e_solidity
{
	solid,
	sensor,
};

// ----------------------------------------------------------------------------

enum struct e_sc_type
{
	dynamic,
	kinematic,
	stationary,
};

// ----------------------------------------------------------------------------

enum struct e_draw_call
{
	opaque,
	transparent,
	max,
};

// ----------------------------------------------------------------------------

enum struct e_tnkf_type
{
	none,
	shake_angle,
	pp_color_overlay,
	pp_pixelate,
	play_sound,
	scene_push_under,
	scene_pop_under,
	reset_scene_stack_to_main_menu,
	max,
};

// ----------------------------------------------------------------------------

enum struct e_sensor_type
{
	one_shot,
	repeating,
	continuous
};
