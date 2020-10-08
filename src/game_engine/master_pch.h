#pragma once

#pragma warning(disable : 26812)	// https://docs.microsoft.com/en-us/visualstudio/code-quality/c26812?view=vs-2019
#pragma warning(disable : 4251)

#pragma warning(push, 0)	// turn off all warnings for external libraries

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

	// Tweeny
	#include "tweeny-master/include/tweeny.h"
	//using tweeny::easing;

	// GLEW provides access to modern OpenGL - everything after v1.1
	#define GLEW_STATIC
	#include <glew-2.1.0/include/GL/glew.h>

	// GLFW makes getting OpenGL up and running a lot easier
	#include <glfw-3.3.2.bin.WIN64/include/GLFW/glfw3.h>
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <glfw-3.3.2.bin.WIN64/include/GLFW/glfw3native.h>
	#undef APIENTRY

	// image loading
	// (https://github.com/nothings/stb)
	#include "stb-master/stb_image.h"

	// Box2D physics
	#include "box2d-master/include/box2d/box2d.h"

	// sound and music library
	//
	// setting this #define means we are using the BASS library
	// and must include "bass.dll" in the game distribution.
	//
	// otherwise, we use cute_sound which uses DirectSound and
	// requires no external files.

	#define USE_BASS_SOUND_LIBRARY

	#ifdef USE_BASS_SOUND_LIBRARY
		#include "bass24/c/bass.h"
	#else
		// sound and music library
		#include "cute_headers/cute_sound.h"
	#endif

	// cute headers
	// (https://github.com/RandyGaul/cute_headers)
	#include "cute_headers/cute_c2.h"

	// {fmt}
	#define FMT_HEADER_ONLY
	#include "fmt/include/fmt/core.h"
	#include "fmt/include/fmt/format.h"
	#include "fmt/include/fmt/format-inl.h"

	// XBox controller + haptics support
	#include <Xinput.h>

#pragma warning(pop)	// turn warnings back on

// ----------------------------------------------------------------------------
// game engine specific stuff that doesn't change often
//
// putting things in here will speed up partial compiles
// ----------------------------------------------------------------------------

#include "master_fwd_decl.h"

// ----------------------------------------------------------------------------
// macros to make enum definitions easier to read

#define enum_begin( name )\
	using e_##name = int;\
	namespace name\
	{\
		enum\
		{

#define enum_end\
		};\
	};

#include "master_enum.h"

#include "stringutil.h"
#include "logfile.h"

#include "mathutil.h"
#include "parseutil.h"
#include "tokenizer.h"

// ----------------------------------------------------------------------------
// handy macros

#define f_commas w_stringutil::format_with_commas
#define log_msg(string_format, ...) logfile->msg( fmt::format("[{}:{}] " ## string_format ## "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__ ) )
#define log_warning(string_format, ...) logfile->msg( fmt::format("[{}:{}] WARNING : " ## string_format ## "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__ ) )
#define log_error(string_format, ...) logfile->error( fmt::format("[{}:{}] ERROR! : " ## string_format ## "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__ ) )

// ----------------------------------------------------------------------------

constexpr float W_PI = 3.14159265358979323846f;

// used to convert color values in the range 0-255 to 0-1
// i.e. color.r = 168.0f * byte_color_to_float;
//
constexpr float byte_color_to_float = 1.0f / 255.0f;

// ----------------------------------------------------------------------------
// standard rendering start depths

constexpr float zdepth_clear_window = -15000.0f;
constexpr float zdepth_nudge = 10.0f;
constexpr float zdepth_background = 0.0f;
constexpr float zdepth_layers = 1000.0f;
constexpr float zdepth_layer_step = 1000.0f;
constexpr float zdepth_stats = 14500.0f;
constexpr float zdepth_topmost = 14750.0f;

// ----------------------------------------------------------------------------

template<typename T>
constexpr T w_min( T a, T b )
{
	return ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) );
}
template<typename T>
constexpr T w_max( T a, T b )
{
	return ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) );
}
template<typename T>
constexpr T w_clamp( T v, T min, T max )
{
	return ( w_max( min, w_min( max, v ) ) );
}

// quickly compare 2 floats to see if they are equal within the epsilon tolerance

constexpr bool fequals( float a, float b )
{
	return ( ( (a) -( b ) ) < FLT_EPSILON && ( (a) -( b ) ) > -FLT_EPSILON );
}

// ----------------------------------------------------------------------------

constexpr float b2d_gravity_default = 9.81f;
constexpr float b2d_world_scale_factor = 100.0f;

constexpr float to_b2d( float v )
{
	return ( v / b2d_world_scale_factor );
}

constexpr float from_b2d( float v )
{
	return ( v * b2d_world_scale_factor );
}

constexpr bool im_left_clicked( e_im_result result )
{
	return ( result & im_result::left_clicked );
}

constexpr bool im_right_clicked( e_im_result result )
{
	return ( result & im_result::right_clicked );
}

// ----------------------------------------------------------------------------

#define MATRIX engine->opengl
#define OPENGL engine->opengl
#define RENDER engine->render
#define UI engine->ui
#define INPUT engine->input

// ----------------------------------------------------------------------------
//	useful type definitions to increase code readability

using w_uv = w_vec2;
using w_sz = w_vec2;
using w_offset = w_vec2;
using w_pos = w_vec2;
using bitflags = unsigned;

// ----------------------------------------------------------------------------

using b_silent = bool;
using b_sensor = bool;
using b_ignore_dead_zone = bool;

// ----------------------------------------------------------------------------

typedef std::variant<c2Circle, c2AABB, c2Capsule> variant_collider_types;

#define ADD_QUOTES_HELPER(s) #s
#define ADD_QUOTES(s) ADD_QUOTES_HELPER(s)
