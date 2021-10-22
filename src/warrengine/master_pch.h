

#include "master_fwd_decl.h"

#pragma warning(disable : 4530 )	// C++ exception handler used, but unwind semantics are not enabled
#pragma warning(disable : 26812 )	// unscoped enum

#pragma warning( push, 0 )	// turn off all warnings for external libraries

	#include <cmath>

	#define WIN32_LEAN_AND_MEAN

	// GLM - math library

	#include <glm/glm/glm.hpp>
	#include <glm/glm/gtc/matrix_transform.hpp>
	#include <glm/glm/gtc/type_ptr.hpp>
	#include <glm/glm/gtc/random.hpp>
	#include <glm/glm/ext/scalar_constants.hpp>
	#include <glm/glm/gtc/noise.hpp>

	// stl
	#include <string>
	#include <string_view>
	#include <vector>
	#include <stack>
	#include <map>
	#include <array>
	#include <filesystem>
	#include <fstream>
	#include <variant>
	#include <future>
	#include <format>
	#include <numeric>
	#include <any>
	#include <unordered_set>

	// SMFL
	#include <SFML/Audio.hpp>		// sound/music playing
	#include <SFML/Graphics.hpp>	// image loading

	// Tweeny
	#include "tweeny-master/include/tweeny.h"

	// GLEW provides access to modern OpenGL
	#define GLEW_STATIC
	#include <glew-2.2.0/include/GL/glew.h>

	// GLFW - window handling, input, time/ticks
	#include <glfw/include/GLFW/glfw3.h>
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <glfw/include/GLFW/glfw3native.h>

	//#define _X86_
	#include "winnt.h"

	// Box2D - physics
	#include "box2d-master/include/box2d/box2d.h"

	// cute headers - simple collision detection
	// (https://github.com/RandyGaul/cute_headers)
	#include "cute_headers/cute_c2.h"

	// controller input + rumble support
	#include <Xinput.h>

#pragma warning(pop)	// turn warnings back on

namespace war
{

// simple data types

using i64 = int64_t;
using ui64 = uint64_t;
using i32 = int32_t;
using ui32 = uint32_t;
using i16 = int16_t;
using ui16 = uint16_t;
using i8 = int8_t;
using ui8 = uint8_t;
using f32 = float_t;
using d64 = double_t;

// ----------------------------------------------------------------------------
// fixed-time stepping constants

namespace fixed_time_step
{
	// how many fixed time steps, per second
	constexpr i32 frames_per_second = 60;

	// how many milliseconds will have passed each time a fixed time step update
	// occurs
	constexpr i32 ms_per_step = (i32)( 1000.f / (f32)frames_per_second );

	// any value you want to update as a "per second" value in an update
	// function should be multiplied against this constant.
	//
	// i.e. if you want to rotate something 15 degrees per second, then in your
	// update function, increase your angle value with something like:
	//
	// angle += 15.f * fixed_time_step::per_second_scaler;

	constexpr f32 _per_second_scaler = 1.f / (f32)frames_per_second;

	template<typename T>
	constexpr T per_second( T val )
	{
		return val * _per_second_scaler;
	}
}

// ----------------------------------------------------------------------------
//	useful type definitions to increase code readability

using time_ms = ui64;

// ----------------------------------------------------------------------------
// strings to validate text input against

const std::string valid_chars_alphabet_upper = "ABCDEFGHJIKLMNOPQRSTUVWXYZ";
const std::string valid_chars_alphabet_lower = "abcdefghjiklmnopqrstuvwxyz";
const std::string valid_chars_numbers = "1234567890";
const std::string valid_chars_alphanumeric = "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyz1234567890";
const std::string valid_chars_simple_whitespace = " -_";
const std::string valid_chars_punctuation = "!@#$%^&*(),.;:";

}

// ----------------------------------------------------------------------------

#include "master_enum.h"

#include "utils.h"
#include "logfile.h"

#include "value_accumulator.h"
#include "parser.h"
#include "tokenizer.h"

namespace war
{

// ----------------------------------------------------------------------------
// flips a boolean variable
// - this is a macro so it works for both bools and bitfields

#define toggle_bool( b ) { (b) = !(b); }

// ----------------------------------------------------------------------------
// used to convert color values in the range 0-255 to 0-1
// i.e. color.r = 168.f * byte_color_to_float;

constexpr f32 byte_color_to_float( i32 value )
{
	return value / 255.f;
}

// ----------------------------------------------------------------------------
// standard rendering start depths
//
// note : larger values are closer to the camera
//
// note : because we flip the Y axis in the projection matrix, these values end
// up getting negated. it's weird but i -think- that's what happens. so while
// these feel like they should be negative numbers, we work with them as
// positive because of the projection flip.
// ----------------------------------------------------------------------------

constexpr f32 zdepth_nudge = 10.f;

constexpr f32 zdepth_background = 100.f;
constexpr f32 zdepth_scene_range = 1000.f;
constexpr f32 zdepth_stats = 45000.f;
constexpr f32 zdepth_topmost = 50000.f;
constexpr f32 zdepth_max = 50000.f;

// ----------------------------------------------------------------------------
// if (a - b) is smaller than epsilon, return true.
//
// this allows f32 values to be compared with some slack for accuracy drift.

static auto epsilon = glm::epsilon<f32>();

[[nodiscard]] constexpr bool fequals( f32 a, f32 b )
{
	auto diff = a - b;
	return ( diff < epsilon && diff > -epsilon );
}

[[nodiscard]] f32 snap_to_int( f32 val );

// ----------------------------------------------------------------------------
// physics constants and helpers
// ----------------------------------------------------------------------------

constexpr auto b2d_gravity_default = 9.81f;
constexpr auto b2d_world_scale_factor = 100.f;

// values yoinked from : https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_hello.html
//
// simple sims need values like : [6, 2]
// more complex sims need : [8, 3]

constexpr auto b2d_velocity_iterations = 8;
constexpr auto b2d_pos_iterations = 3;

[[nodiscard]] constexpr f32 to_box2d( f32 v )
{
	return ( v / b2d_world_scale_factor );
}

[[nodiscard]] constexpr f32 from_box2d( f32 v )
{
	return ( v * b2d_world_scale_factor );
}

// ----------------------------------------------------------------------------
// simple collision constants and helpers
// ----------------------------------------------------------------------------

constexpr auto simple_collision_gravity_default = 9.81f;
constexpr auto simple_world_scale_factor = 100.f;

[[nodiscard]] constexpr f32 to_simple( f32 v )
{
	return ( v / simple_world_scale_factor );
}

[[nodiscard]] constexpr f32 from_simple( f32 v )
{
	return ( v * simple_world_scale_factor );
}

// ----------------------------------------------------------------------------

extern std::unique_ptr<war::Log_File> g_logfile;
extern std::unique_ptr<Game> g_base_game;

// ----------------------------------------------------------------------------
// attempts to make logging things easier
//
// i.e. log( "a log entry" );
// ----------------------------------------------------------------------------

template<typename ...Params>
constexpr void _log_( Params&&... params )
{
	g_logfile->msg( std::format( std::forward<Params>( params )... ) );
}

template<typename ...Params>
constexpr void _verbose_( Params&&... params )
{
	g_logfile->verbose( std::format( std::forward<Params>( params )... ) );
}

template<typename ...Params>
constexpr void _log_fatal_( Params&&... params )
{
	g_logfile->error( std::format( std::forward<Params>( params )... ) );
}

// note : you will be tempted to convert these defines to constexpr templates
// but if you do that, we stop getting accurate reporting of __FUNCTION__ and
// __LINE__ macros, which makes the log less useful.
//
// maybe revisit this when std::source_location comes to MSVC

#define log( fmt, ... ) _log_( "[{}:{}] " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define log_warning( fmt, ... ) _log_( "[{}:{}] WARNING : " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define log_fatal( fmt, ... ) _log_fatal_( "[{}:{}] !ERROR! : " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define log_div() _log_( "[{}:{}] ----------------------------------------\n", __FUNCTION__, __LINE__ )
#define log_verbose( fmt, ... ) _verbose_( "[{}:{}] " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )

// ----------------------------------------------------------------------------
// what:
//
// this is a super simple hasher for string literals, but all it's for is to
// generate unique ui32 values for arbitrary string literals that are
// passed to it. it also needs to be constexpr friendly so it melts away in
// release builds. this allows the engine to compare ui32s instead of
// strings in places like the UI code, which is WAY faster.
//
// how:
//
// it recursively reads each character in the string literal and adds it's
// "value * (index + 1)" into the final hash value. incorporating the index is
// necessary as it removes false positives that COULD happen with transposed
// characters in strings which WOULD happen if we simply added up the straight
// ASCII value of each character.
//
// in other words H("AB") != H("BA"), which is what we want.

using hash = ui32;
constexpr hash hash_none = 0;

constexpr hash hash_it( const char* str )
{
	hash result = hash_none;

	const char* rp = str;
	i32 i = 0;

	while( *rp )
	{
		result += ( *rp ) * i;
		i++;
		rp++;
	}

	return result;
}

#define H(str) ( hash_it( str ) )

// ----------------------------------------------------------------------------
// the size of the viewport. all rendering and coordinate converions are done on
// the assumption of these dimensions. the renderer handles scaling this up to
// the actual game window the player sees. this makes a lot of things simpler.

extern f32 viewport_w;
extern f32 viewport_h;

#define viewport_hw (viewport_w * 0.5f)
#define viewport_hh (viewport_h * 0.5f)

// the size to render the UI at. this allows the game to run at a different
// resolution from the UI.
//
// this has advantages like letting you author UI graphics at a specific
// resolution and having them work across multiple games.

extern f32 ui_w;
extern f32 ui_h;

#define ui_hw (ui_w * 0.5f)
#define ui_hh (ui_h * 0.5f)

#define ui_scale (viewport_w / ui_w)

// the final res that we want to present the screen at. this is useful for cases
// where you want to render the game at a higher res but actually present it to
// the user at a smaller res. or you're in a 128x128 game jam, or what-not.

extern f32 final_pixel_w;
extern f32 final_pixel_h;

// ----------------------------------------------------------------------------

#define make_color war::Color::make

}
