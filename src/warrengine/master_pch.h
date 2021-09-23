

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
	#include <map>
	#include <array>
	#include <filesystem>
	#include <fstream>
	#include <variant>
	#include <future>
	#include <format>
	#include <numeric>
	#include <any>

	// SMFL
	#include <SFML/Audio.hpp>		// sound/music playing
	#include <SFML/Graphics.hpp>	// image loading

	// Tweeny
	#include "tweeny-master/include/tweeny.h"

	// GLEW provides access to modern OpenGL
	#define GLEW_STATIC
	#include <glew-2.2.0/include/GL/glew.h>

	// GLFW - window handling, input, time/ticks
	#include <glfw-3.3.2.bin.WIN64/include/GLFW/glfw3.h>
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <glfw-3.3.2.bin.WIN64/include/GLFW/glfw3native.h>

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

// ----------------------------------------------------------------------------
// fixed-time stepping constants

namespace fixed_time_step
{
	// how many fixed time steps, per second
	constexpr int32_t frames_per_second = 60;

	// how many milliseconds will have passed each time a fixed time step update
	// occurs
	constexpr int32_t ms_per_step = (int32_t)( 1000.f / (float_t)frames_per_second );

	// any value you want to update as a "per second" value in an update
	// function should be multiplied against this constant.
	//
	// i.e. if you want to rotate something 15 degrees per second, then in your
	// update function, increase your angle value with something like:
	//
	// angle += 15.f * fixed_time_step::per_second_scaler;

	constexpr float_t _per_second_scaler = 1.f / (float_t)frames_per_second;

	template<typename T>
	constexpr T per_second( T val )
	{
		return val * _per_second_scaler;
	}
}

// ----------------------------------------------------------------------------
//	useful type definitions to increase code readability

using time_ms = uint64_t;

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

constexpr float_t byte_color_to_float( int32_t value )
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

constexpr float_t zdepth_nudge = 10.f;

constexpr float_t zdepth_background = 100.f;
constexpr float_t zdepth_scene_start = 1000.f;
constexpr float_t zdepth_stats = 45000.f;
constexpr float_t zdepth_topmost = 50000.f;
constexpr float_t zdepth_max = 50000.f;

constexpr float_t zdepth_debug_bias = 1000.f;

// ----------------------------------------------------------------------------
// if (a - b) is smaller than epsilon, return true.
//
// this allows float_t values to be compared with some slack for accuracy drift.

static auto epsilon = glm::epsilon<float_t>();

[[nodiscard]] constexpr bool fequals( float_t a, float_t b )
{
	auto diff = a - b;
	return ( diff < epsilon && diff > -epsilon );
}

[[nodiscard]] float_t snap_to_int( float_t val );

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

[[nodiscard]] constexpr float_t to_box2d( float_t v )
{
	return ( v / b2d_world_scale_factor );
}

[[nodiscard]] constexpr float_t from_box2d( float_t v )
{
	return ( v * b2d_world_scale_factor );
}

// ----------------------------------------------------------------------------
// simple collision constants and helpers
// ----------------------------------------------------------------------------

constexpr auto simple_collision_gravity_default = 9.81f;
constexpr auto simple_world_scale_factor = 100.f;

[[nodiscard]] constexpr float_t to_simple( float_t v )
{
	return ( v / simple_world_scale_factor );
}

[[nodiscard]] constexpr float_t from_simple( float_t v )
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
// generate unique uint32_t values for arbitrary string literals that are
// passed to it. it also needs to be constexpr friendly so it melts away in
// release builds. this allows the engine to compare uint32_ts instead of
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

using hash = uint32_t;
constexpr hash hash_none = 0;

constexpr hash hash_it( const char* str )
{
	hash result = hash_none;

	const char* rp = str;
	int32_t i = 0;

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

extern float_t viewport_w;
extern float_t viewport_h;

#define viewport_hw (viewport_w * 0.5f)
#define viewport_hh (viewport_h * 0.5f)

extern float_t ui_w;
extern float_t ui_h;

#define ui_hw (ui_w * 0.5f)
#define ui_hh (ui_h * 0.5f)

#define ui_scale (viewport_w / ui_w)

// ----------------------------------------------------------------------------

#define make_color war::Color::make

}
