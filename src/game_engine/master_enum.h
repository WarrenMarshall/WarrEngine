
#pragma once

// ----------------------------------------------------------------------------

enum_begin( align )
	invalid = 0,
	left = 1,
	right = 2,
	hcenter = 4,
	vcenter = 8,
	horizontal = 16,
	vertical = 32,
	centered = hcenter | vcenter
enum_end

// ----------------------------------------------------------------------------

enum_begin( color )
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
enum_end

// ----------------------------------------------------------------------------

enum_begin( timeline_type )
	invalid = 0,
	float_type,
	color_type
enum_end

// ----------------------------------------------------------------------------

enum_begin( input_id )
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

	key_a,	key_b,	key_c,	key_d,	key_e,	key_f,	key_g,
	key_h,	key_i,	key_j,	key_k,	key_l,	key_m,	key_n,
	key_o,	key_p,	key_q,	key_r,	key_s,	key_t,	key_u,
	key_v,	key_w,	key_x,	key_y,	key_z,
	key_0,	key_1,	key_2,	key_3,	key_4,	key_5,	key_6,
	key_7,	key_8,	key_9,
	key_left_bracket,
	key_right_bracket,
	key_esc,
	key_enter,
	key_space,
	key_f1,	key_f2,	key_f3,	key_f4,		key_f5,		key_f6,
	key_f7,	key_f8,	key_f9,	key_f10,	key_f11,	key_f12,
	key_up,	key_down, key_left, key_right,
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
	key_delete,

	key_shift_left,
	key_shift_right,
	key_control_left,
	key_control_right,
	key_alt_left,
	key_alt_right,

	max
enum_end

// ----------------------------------------------------------------------------

#undef small	// fixes dumb compile problem

enum_begin( rumble_effect )
	tiny = 0,
	small,
	medium,
	large
enum_end

// ----------------------------------------------------------------------------

enum_begin( life_cycle )
	invalid = 0,
	alive,
	dying,			// pending delete whenever the object is finished doing everything (components are finished, etc)
	dead			// delete next update iteration, this object isn't needed anymore and has finished all of it's work
enum_end

// ----------------------------------------------------------------------------

enum_begin( event_id )
	invalid = 0,
	input_pressed,
	input_released,
	input_held,
	input_motion
enum_end

// ----------------------------------------------------------------------------

enum_begin( tween_type )
	linear = 1,		// value iterates from start to end, once
	loop,			// value iterates from start to end and resets to start, looping
	pingpong,		// value iterates from start to end and back again, looping
	half_sine,		// uses half a sin wave to bounce
	sine			// a full sin wave
enum_end

// ----------------------------------------------------------------------------
//	a definition for the source coordinates of a 9-slice image
//
// these are stored row/column:
//
// 00 10 20
// 01 11 21
// 02 12 22

enum_begin( slicedef_patch )
	P_00 = 0, P_10, P_20,
	P_01, P_11, P_21,
	P_02, P_12, P_22
enum_end

// ----------------------------------------------------------------------------

enum_begin( opengl_blend )
	alpha = 0,
	add,
	multiply
enum_end

// ----------------------------------------------------------------------------

enum_begin( particle_spawn_dir )
	inherit_from_parent = -1,
	away_from_center = -2
enum_end

// ----------------------------------------------------------------------------
// normal = no custom cursor image, using OS mouse pointer
// hidden = custom cursor image, OS mouse pointer is hidden
// locked = no cursor showing at all, mouse is locked to game window

enum_begin( mouse_mode )
	normal = 0,
	hidden,
	locked
enum_end

// ----------------------------------------------------------------------------

enum_begin( button_state )
	invalid = 0,
	pressed,
	held,
	released,
	up,
	max
enum_end

// ----------------------------------------------------------------------------

enum_begin( im_result )
	none = 0,
	hovered = 1,
	hot = 2,
	left_clicked = 4,
	right_clicked = 8
enum_end

// ----------------------------------------------------------------------------

enum_begin( collision_layer )
	//bit1 = 1,		// don't use this, as everything IS this by default in Box2D
	bit2 = 2,
	bit3 = 4,
	bit4 = 8,
	bit5 = 16,
	bit6 = 32,
	bit7 = 64,
	bit8 = 128,
	bit9 = 256,
	bit10 = 512,
	bit11 = 1024,
	bit12 = 2048,
	bit13 = 4096,
	bit14 = 8192,
	bit15 = 16384,
	bit16 = 32768
	enum_end

// ----------------------------------------------------------------------------

enum_begin( component_type )
	invalid = 0,
	sprite = 1,
	emitter = 2,
	sound = 4,
	b2d_body = 8,
	b2d_static = 16,
	b2d_dynamic = 32,
	b2d_kinematic = 64
enum_end
