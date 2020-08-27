#pragma once

#pragma warning(disable : 26812)	// https://docs.microsoft.com/en-us/visualstudio/code-quality/c26812?view=vs-2019
#pragma warning(disable : 4251)

#pragma warning(push, 0)	// turn off all warnings for external libraries

	#include <glm\glm.hpp>
	#include <glm\gtc\matrix_transform.hpp>
	#include <glm\gtc\type_ptr.hpp>

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
	#include <type_traits>

	// GLEW provides access to modern OpenGL - everything after v1.1
	#define GLEW_STATIC
	#include <GL/glew.h>

	// GLFW makes getting OpenGL up and running a lot easier
	#include <GLFW/glfw3.h>
	#undef APIENTRY

	// image loading
	// (https://github.com/nothings/stb)
	#include "stb_image.h"

	// cute headers
	// (https://github.com/RandyGaul/cute_headers)
	#include "cute_c2.h"
	#include "cute_sound.h"
	#include "cute_sound_pitch_plugin.h"

	// sound and music library
	#include <bass.h>

	// {fmt}
	#define FMT_HEADER_ONLY
	#include "fmt/core.h"
	#include "fmt/format.h"
	#include "fmt/format-inl.h"

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

//#define s_format fmt::format
#define s_commas w_stringutil::format_with_commas
#define log_msg logfile->msg
#define log_warning logfile->warning
#define log_error logfile->error

// ----------------------------------------------------------------------------

constexpr int num_asset_def_passes = 3;
constexpr float W_PI = 3.14159265358979323846f;

// ----------------------------------------------------------------------------
// standard rendering start depths

constexpr float zdepth_nudge = 10.0f;
constexpr float zdepth_background = 0.0f;
constexpr float zdepth_layers = 1000.0f;
constexpr float zdepth_layer_step = 500.0f;
constexpr float zdepth_stats = 9000.0f;
constexpr float zdepth_engine = 9500.0f;

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
//#define fequals(a,b)

constexpr bool fequals( float a, float b )
{
	return ( ( (a) -( b ) ) < FLT_EPSILON && ( (a) -( b ) ) > -FLT_EPSILON );
}

// ----------------------------------------------------------------------------

#define MATRIX engine->opengl
#define OPENGL engine->opengl
#define RENDER engine->render
#define UI engine->ui

// ----------------------------------------------------------------------------
//	useful type definitions to increase code readability

using w_uv = w_vec2;
using w_sz = w_vec2;
using w_offset = w_vec2;
using w_pos = w_vec2;

// ----------------------------------------------------------------------------

using b_silent = bool;
using b_draw_button = bool;

#define str_not_found "n/a"
