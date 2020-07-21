#pragma once

// ----------------------------------------------------------------------------

enum struct e_align : int
{
	invalid = 0,
	left = 1,
	right = 2,
	hcenter = 4,
	vcenter = 8,
	horizontal = 16,
	vertical = 32,
	centered = hcenter | vcenter
};
DEFINE_ENUM_FLAG_OPERATOR( e_align )

// ----------------------------------------------------------------------------

enum struct e_color : int
{
	white = 1,
	black,
	red,
	green,
	blue,
	light_blue,
	orange,
	yellow,
	teal,
	dark_teal,
	dark_grey,
	grey,
	light_grey
};

// ----------------------------------------------------------------------------

enum struct e_timeline_type : int
{
	invalid = 0,
	float_type,
	color_type
};

// ----------------------------------------------------------------------------
// event IDs for the UI system

enum struct e_ui_event : int
{
	none = 0,
	play_clicked,
	exit_clicked,
	back_clicked,
	advance_clicked,
	realtime_clicked,
	clear_clicked,
	slower_clicked,
	faster_clicked
};

// ----------------------------------------------------------------------------

enum struct e_input_id : int
{
	invalid = 0,
	controller_button_dpad_up,
	controller_button_dpad_down,
	controller_button_dpad_left,
	controller_button_dpad_right,
	controller_button_start,
	controller_button_guide,
	controller_button_back,
	controller_button_left_thumb,		// this is a button because you can click the stick in
	controller_button_right_thumb,
	controller_button_left_shoulder,
	controller_button_right_shoulder,
	controller_button_a,
	controller_button_b,
	controller_button_x,
	controller_button_y,

	controller_left_stick,				// this is for reading the axis movement values
	controller_right_stick,
	controller_left_trigger,
	controller_right_trigger,

	mouse_button_left,
	mouse_button_middle,
	mouse_button_right,
	mouse,

	key_a,
	key_b,
	key_c,
	key_d,
	key_e,
	key_f,
	key_g,
	key_h,
	key_i,
	key_j,
	key_k,
	key_l,
	key_m,
	key_n,
	key_o,
	key_p,
	key_q,
	key_r,
	key_s,
	key_t,
	key_u,
	key_v,
	key_w,
	key_x,
	key_y,
	key_z,
	key_0,
	key_1,
	key_2,
	key_3,
	key_4,
	key_5,
	key_6,
	key_7,
	key_8,
	key_9,
	key_left_bracket,
	key_right_bracket,
	key_esc,
	key_enter,
	key_space,
	key_f1,
	key_f2,
	key_f3,
	key_f4,
	key_f5,
	key_f6,
	key_f7,
	key_f8,
	key_f9,
	key_f10,
	key_f11,
	key_f12,
	key_up,
	key_down,
	key_left,
	key_right,
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

	max
};

// ----------------------------------------------------------------------------

#undef small	// fixes dumb compile problem in release

enum struct e_rumble_effect : int
{
	tiny,
	small,
	medium,
	large
};

// ----------------------------------------------------------------------------

enum struct e_lifecycle : int
{
	invalid = 0,
	alive,
	dying,			// pending delete whenever the object is finished doing everything (components are finished, etc)
	dead			// delete next update iteration, this object isn't needed anymore and has finished all of it's work
};

// ----------------------------------------------------------------------------

enum struct e_event_id : int
{
	invalid,
	
	emitter_params_hot_reload,

	input_pressed,
	input_released,
	input_motion
};

// ----------------------------------------------------------------------------

enum struct e_tween_type : int
{
	linear = 1,		// value iterates from start to end, once
	loop,			// value iterates from start to end and resets to start, looping
	pingpong,		// value iterates from start to end and back again, looping
	half_sine,		// uses half a sin wave to bounce
	sine			// a full sin wave
};

// ----------------------------------------------------------------------------
//	a definition for the source coordinates of a 9-slice image

enum struct e_patch : int
{
	P_00 = 0,
	P_10,
	P_20,
	P_01,
	P_11,
	P_21,
	P_02,
	P_12,
	P_22
};

// ----------------------------------------------------------------------------
//	opengl blending modes

enum struct e_opengl_blend : unsigned int
{
	alpha,
	add,
	multiply
};

// ----------------------------------------------------------------------------

enum struct e_render_pass : int
{
	solid = 0,
	transparent
};

// ----------------------------------------------------------------------------

enum struct e_component_type : int
{
	invalid = 0,
	sprite,
	emitter,
	sound
};

// ----------------------------------------------------------------------------

enum struct e_particle_spawn_dir : int
{
	inherit_from_parent = -1,
	away_from_parent = -2
};

// ----------------------------------------------------------------------------

enum struct e_mouse_mode : int
{
	normal,
	hidden,
	locked
};

// ----------------------------------------------------------------------------

enum struct e_ui_id : int
{
	invalid = 0,
	browse,
	tile_start,
	tile_end = tile_start + ( 19 * 9 ),
	max
};

// ----------------------------------------------------------------------------

enum struct e_button_state : int
{
	invalid = 0,
	pressed,
	held,
	released,
	up,
	max
};
