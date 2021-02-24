#pragma once
#define WIN32_LEAN_AND_MEAN

#include "master_fwd_decl.h"

#pragma warning(disable : 4324 )	// structure was padded due to alignment specifier
#pragma warning(disable : 4530 )	// C++ exception handler used, but unwind semantics are not enabled
#pragma warning(disable : 4130 )	// logical operation on address of string constant
#pragma warning(disable : 26812 )

#pragma warning( push, 0 )	// turn off all warnings for external libraries

	#pragma warning(disable : 4244)	// possible loss of data

	// GLM - math library
	#include <glm/glm/glm.hpp>
	#include <glm/glm/gtc/matrix_transform.hpp>
	#include <glm/glm/gtc/type_ptr.hpp>

	#include <cassert>
	#include <cstdio>
	#include <cstdlib>
	#include <iostream>
	#include <ctime>
	#include <typeinfo>
	#include <cmath>

	// stl
	#include <random>
	#include <string>
	#include <string_view>
	#include <vector>
	#include <deque>
	#include <stack>
	#include <map>
	#include <set>
	#include <array>
	#include <algorithm>
	#include <thread>
	#include <mutex>
	#include <atomic>
	#include <filesystem>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <locale>
	#include <variant>
	#include <memory>
	#include <optional>
	#include <future>
	#include <numbers>
	#include <span>

	// SMFL
	#include <SFML/Audio.hpp>
	#include <SFML/Graphics.hpp>

	// Tweeny
	#include "tweeny-master/include/tweeny.h"

	// GLEW provides access to modern OpenGL - everything after v1.1
	#define GLEW_STATIC
	#include <glew-2.2.0/include/GL/glew.h>

	// GLFW makes getting OpenGL up and running a lot easier
	#include <glfw-3.3.2.bin.WIN64/include/GLFW/glfw3.h>
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <glfw-3.3.2.bin.WIN64/include/GLFW/glfw3native.h>
	#undef APIENTRY

	// Box2D - physics
	#include "box2d-master/include/box2d/box2d.h"

	// cute headers - collision detection
	// (https://github.com/RandyGaul/cute_headers)
	#include "cute_headers/cute_c2.h"

	// {fmt}
	#define FMT_HEADER_ONLY
	#include "fmt/include/fmt/core.h"
	#include "fmt/include/fmt/format.h"
	#include "fmt/include/fmt/format-inl.h"

	// XBox controller + haptics support
	#include <Xinput.h>

	// PugiXML
	#define PUGIXML_HEADER_ONLY
	#include "pugixml-1.10/src/pugixml.hpp"

#pragma warning(pop)	// turn warnings back on

// ----------------------------------------------------------------------------
// game engine specific stuff that doesn't change
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// fixed-time stepping constants

namespace fixed_time_step
{
	// how many fixed time steps, per second
	constexpr int frames_per_second = 60;

	// how many milliseconds will have passed each time a fixed time step update
	// occurs
	constexpr int ms_per_step = static_cast<int>( 1000.0f / (float)frames_per_second );

	// any value you want to update as a "per second" value in an update
	// function should be multiplied against this constant.
	//
	// i.e. if you want to rotate something 15 degrees per second, then in your
	// update function, increase your angle value with something like:
	//
	// angle += 15.0f * FTS::per_second_scaler;

	constexpr float per_second_scaler = ms_per_step / 1000.f;
}

// ----------------------------------------------------------------------------

#define declare_find_func( struct_name )\
static struct_name* find( const std::string_view name, const bool silent = false );

#define implement_find_func( struct_name )\
struct_name* struct_name::find( std::string_view name, bool silent )\
{\
	return engine->find_asset<struct_name>( name, silent );\
}

// ----------------------------------------------------------------------------
//	useful type definitions to increase code readability

using w_uv = w_vec2;
using w_sz = w_vec2;
using w_offset = w_vec2;
using w_pos = w_vec2;
using time_ms = uint64_t;
using w_keyframe_data = std::variant<float, w_color>;

// ----------------------------------------------------------------------------
// strings to validate text input against

const std::string valid_chars_alphabet_upper = "ABCDEFGHJIKLMNOPQRSTUVWXYZ";
const std::string valid_chars_alphabet_lower = "abcdefghjiklmnopqrstuvwxyz";
const std::string valid_chars_numbers = "1234567890";
const std::string valid_chars_alphanumeric = "ABCDEFGHJIKLMNOPQRSTUVWXYZabcdefghjiklmnopqrstuvwxyz1234567890";
const std::string valid_chars_simple_whitespace = " -_";
const std::string valid_chars_punctuation = "!@#$%^&*(),.;:";

// ----------------------------------------------------------------------------

#include "master_enum.h"

#include "util_string.h"
#include "logfile.h"

#include "accum_value.h"
#include "coord.h"
#include "parser.h"
#include "tokenizer.h"

// ----------------------------------------------------------------------------
// handy macros
// ----------------------------------------------------------------------------

#define f_commas w_string_util::format_with_commas

// ----------------------------------------------------------------------------
// used to convert color values in the range 0-255 to 0-1
// i.e. color.r = 168.0f * byte_color_to_float;
// ----------------------------------------------------------------------------

constexpr float byte_color_to_float = 1.0f / 255.0f;

// ----------------------------------------------------------------------------
// standard rendering start depths
// ----------------------------------------------------------------------------

constexpr float zdepth_clear_window = -15000.0f;
constexpr float zdepth_nudge = 1.0f;
constexpr float zdepth_background = 0.0f;
constexpr float zdepth_layers = 1000.0f;
constexpr float zdepth_layer_step = 1000.0f;
constexpr float zdepth_stats = 14500.0f;
constexpr float zdepth_topmost = 14750.0f;

// ----------------------------------------------------------------------------

// quickly compare 2 floats to see if they are equal within the epsilon tolerance
[[nodiscard]] constexpr bool fequals( float a, float b )
{
	// if (a - b) is smaller than FLT_EPSILON, return true.
	//
	// this allows float values to be compared with some slack for accuracy
	// drift.

	return ( ( (a) -( b ) ) < FLT_EPSILON && ( (a) -( b ) ) > -FLT_EPSILON );
}

[[nodiscard]] float sign_of( float val );
[[nodiscard]] float snap_to_pixel( float f );
[[nodiscard]] int str_to_int( const char* str );
[[nodiscard]] int str_to_int( const std::string& str );
[[nodiscard]] unsigned str_to_uint( const char* str );
[[nodiscard]] unsigned str_to_uint( const std::string& str );

[[nodiscard]] float sin_from_angle( float angle_in_degrees );

// ----------------------------------------------------------------------------
// box2d constants and helpers
// ----------------------------------------------------------------------------

constexpr float b2d_gravity_default = 9.81f;
constexpr float b2d_world_scale_factor = 100.0f;
constexpr int32 b2d_velocity_iterations = 6;
constexpr int32 b2d_pos_iterations = 2;

constexpr float to_b2d( float v )
{
	return ( v / b2d_world_scale_factor );
}

constexpr float from_b2d( float v )
{
	return ( v * b2d_world_scale_factor );
}

// ----------------------------------------------------------------------------
// handy macros
// ----------------------------------------------------------------------------

#define render_state (*w_render::top_render_state)
