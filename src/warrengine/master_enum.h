
// ----------------------------------------------------------------------------
// macros to make enum definitions easier to read.
//
// we do it this way because using the standard enum declarations in C++ makes
// the compiler spew warnings about conversions to and from int.
//
// usage example:
//
//	enum_begin( color )
//		red,
//		white,
//		blue
//	enum_end
//
// this create an integer alias called "e_color" which can be used in place of
// "int" when defining vars using this enum.
//
// to access the enum values, use "clr::white" format.
//
// i.e.
//	e_color border_clr = clr::white;
//
//	void set_screen_color( e_color clr );
// ----------------------------------------------------------------------------

#define enum_begin( name )\
	using e_##name = int;\
	namespace name\
	{\
		enum\
		{

#define enum_end\
		};\
	};

// ----------------------------------------------------------------------------
// note : needs to match values in data/warrengine/preproc.asset_def

enum_begin( align )
	fill		= 0b00000000,
	left		= 0b00000001,
	right		= 0b00000010,
	top			= 0b00000100,
	bottom		= 0b00001000,
	hcenter		= 0b00010000,
	vcenter		= 0b00100000,
	horizontal	= 0b01000000,
	vertical	= 0b10000000,
	centered = hcenter | vcenter,
	top_left = top | left,
enum_end

// ----------------------------------------------------------------------------
// note : needs to match values in data/warrengine/preproc.asset_def

enum_begin( color_preset )
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
enum_end

// ----------------------------------------------------------------------------

#undef small	// fixes dumb compile problem

enum_begin( rumble_effect )
	none = 0,
	tiny,
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
	input_motion,
	input_key
enum_end

// ----------------------------------------------------------------------------
// note : needs to match values in data/warrengine/preproc.asset_def

enum_begin( tween_type )
	invalid = -1,
	one_shot,		// value iterates from start to end, once. when complete, it sits at the end forever.
	loop,			// value iterates from start to end and resets to start, looping
	pingpong		// value iterates from start to end and back again, looping
enum_end

enum_begin( tween_via )
	invalid = -1,
	linear = 0,
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
enum_end

// ----------------------------------------------------------------------------
//	a definition for the source coordinates of a 9-slice image
//
// these are stored row/column:
//
// top_left    top_middle    top_right
// middle_left middle_middle middle_right
// bottom_left bottom_middle bottom_right

enum_begin( slicedef_patch )
	top_left = 0,
	top_middle,
	top_right,

	middle_left,
	middle_middle,
	middle_right,

	bottom_left,
	bottom_middle,
	bottom_right
enum_end

// ----------------------------------------------------------------------------

enum_begin( opengl_blend )
	alpha = 0,
	add,
	multiply,
	glow,
enum_end

// ----------------------------------------------------------------------------
// note : needs to match values in data/warrengine/preproc.asset_def

enum_begin( particle_spawn_dir )
	inherit_from_owner = -1,
	away_from_owner = -2
enum_end

// ----------------------------------------------------------------------------
// os = no custom cursor image, using OS mouse pointer
// custom = custom cursor image, OS mouse pointer is hidden
// locked = no cursor showing at all, mouse is locked to game window

enum_begin( mouse_mode )
	os = 0,
	custom,
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
	none			= 0b00000000,
	hovered			= 0b00000001,
	hot				= 0b00000010,
	left_clicked	= 0b00000100,
enum_end

// ----------------------------------------------------------------------------

enum_begin( collision_mask )
//	bit1	= 0b0000000000000001,		// don't use this, as EVERYTHING is this by default in Box2D
	bit2	= 0b0000000000000010,
	bit3	= 0b0000000000000100,
	bit4	= 0b0000000000001000,
	bit5	= 0b0000000000010000,
	bit6	= 0b0000000000100000,
	bit7	= 0b0000000001000000,
	bit8	= 0b0000000010000000,
	bit9	= 0b0000000100000000,
	bit10	= 0b0000001000000000,
	bit11	= 0b0000010000000000,
	bit12	= 0b0000100000000000,
	bit13	= 0b0001000000000000,
	bit14	= 0b0010000000000000,
	bit15	= 0b0100000000000000,
	bit16	= 0b1000000000000000,
enum_end

// ----------------------------------------------------------------------------

enum_begin( follow_flags )
	none	= 0b00000000,
	x_axis	= 0b00000001,
	y_axis	= 0b00000010,
	angle	= 0b00000100,	// match rotation as well as position

	xy_axis = x_axis | y_axis,
enum_end

// ----------------------------------------------------------------------------

enum_begin( ui_control_type )
	none = 0,
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
	tab,			// a series of clickable tabs that can break screens up into sections
enum_end

// ----------------------------------------------------------------------------

enum_begin( primitive_shape )
	rect = 1,
	filled_rect,
	circle,
	filled_circle,
	point
enum_end

// ----------------------------------------------------------------------------

enum_begin( render_prim )
	quad = 0,
	triangle,
	line,
	point
enum_end

// ----------------------------------------------------------------------------

enum_begin( particle_spawner_type )
	point = 1,
	box,
	in_circle,
	on_circle,
enum_end

// ----------------------------------------------------------------------------

enum_begin( tiling )
	clamp = 0,
	repeat,
enum_end

// ----------------------------------------------------------------------------

enum_begin( tile_flags )
	flipped_horizontally	= 0b00000001,
	flipped_vertically		= 0b00000010,
	flipped_diagonally		= 0b00000100,
enum_end

// ----------------------------------------------------------------------------

enum_begin( noise_type )
	simplex = 0,
	perlin,
enum_end

// ----------------------------------------------------------------------------

enum_begin( to_matrix )
	pos		= 0b00000001,
	angle	= 0b00000010,
	scale	= 0b00000100,
	all		= 0b11111111,
enum_end

// ----------------------------------------------------------------------------

enum_begin( corner )
	all = 0,
	top_left,
	top_right,
	bottom_right,
	bottom_left,
enum_end

// ----------------------------------------------------------------------------

enum_begin( pal )
	darkest = 0,
	darker,
	middle,
	brighter,
	brightest,
enum_end

// ----------------------------------------------------------------------------

enum_begin( cut )
	left = 0,
	right,
	top,
	bottom,
enum_end

// ----------------------------------------------------------------------------

enum_begin( framebuffer )
	color = 0,
	glow,
	pick_ids,
	blurred,
	composited,
	final,
	max,
enum_end

// ----------------------------------------------------------------------------

enum_begin( simple_collision_type )
	aabb = 0,
	circle,
	polygon,
enum_end

// ----------------------------------------------------------------------------

enum_begin( simple_collider_type )
	solid = 0,
	sensor,
enum_end

// ----------------------------------------------------------------------------

enum_begin( batch_type )
	opaque = 0,
	transparent,
	max,
enum_end

