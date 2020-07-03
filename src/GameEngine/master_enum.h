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

	keyboard_a,
	keyboard_b,
	keyboard_c,
	keyboard_d,
	keyboard_e,
	keyboard_f,
	keyboard_g,
	keyboard_h,
	keyboard_i,
	keyboard_j,
	keyboard_k,
	keyboard_l,
	keyboard_m,
	keyboard_n,
	keyboard_o,
	keyboard_p,
	keyboard_q,
	keyboard_r,
	keyboard_s,
	keyboard_t,
	keyboard_u,
	keyboard_v,
	keyboard_w,
	keyboard_x,
	keyboard_y,
	keyboard_z,
	keyboard_0,
	keyboard_1,
	keyboard_2,
	keyboard_3,
	keyboard_4,
	keyboard_5,
	keyboard_6,
	keyboard_7,
	keyboard_8,
	keyboard_9,
	keyboard_left_bracket,
	keyboard_right_bracket,
	keyboard_esc,
	keyboard_enter,
	keyboard_space,
	keyboard_f1,
	keyboard_f2,
	keyboard_f3,
	keyboard_f4,
	keyboard_f5,
	keyboard_f6,
	keyboard_f7,
	keyboard_f8,
	keyboard_f9,
	keyboard_f10,
	keyboard_f11,
	keyboard_f12,
	keyboard_up,
	keyboard_down,
	keyboard_left,
	keyboard_right,
	keyboard_period,
	keyboard_comma,
	keyboard_slash,
	keyboard_at,
	keyboard_tilde,
	keyboard_quote,
	keyboard_double_quote,
	keyboard_backspace,
	keyboard_exclaim,
	keyboard_hash,
	keyboard_percent,
	keyboard_dollar,
	keyboard_ampersand,
	keyboard_and,
	keyboard_asterisk,
	keyboard_left_paren,
	keyboard_right_paren,
	keyboard_less,
	keyboard_equals,
	keyboard_greater,
	keyboard_question,
	keyboard_caret,
	keyboard_underscore,
	keyboard_backslash,
	keyboard_backquote,
	keyboard_pause,

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

enum struct e_opaque : int
{
	draw = 1,
	input = 2
};
DEFINE_ENUM_FLAG_OPERATOR( e_opaque )

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
	opaque = 0,
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
