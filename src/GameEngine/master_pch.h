#pragma once

#pragma warning(disable : 26444)	// disabling : https://docs.microsoft.com/en-us/visualstudio/code-quality/c26444?view=vs-2019
#pragma warning(disable : 6011)

#pragma warning(push, 0)	// turn off all warnings for external libraries

	#include <glm\glm.hpp>
	#include <glm\gtc\matrix_transform.hpp>
	#include <glm\gtc\type_ptr.hpp>

	#include <assert.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <iostream> 
	#include <time.h> 
	#include <typeinfo>
	#include <cmath>

	// stl
	#include <string>
	#include <vector>
	#include <deque>
	#include <stack>
	#include <map>
	#include <set>
	#include <array>
	#include <algorithm>
	#include <thread>
	#include <atomic>
	#include <filesystem>
	#include <fstream>
	#include <sstream>
	#include <iomanip>
	#include <locale>

	// GLEW provides access to modern OpenGL - everything after v1.1
	#define GLEW_STATIC
	#include <GL/glew.h>

	// GLFW makes getting OpenGL up and running a lot easier
	#include <GLFW/glfw3.h>
	#undef APIENTRY

	// image loading
	#include "stb_image.h"

	// sound and music library
	#include <bass.h>

	// XBox controller + haptics support
	#include <Xinput.h>

#pragma warning(pop)	// turn warnings back on

using namespace std::literals::chrono_literals;

// ----------------------------------------------------------------------------

/*
	this was taken from the internet. it apparently used to be included in winnt.h
	and it allows you to use bitmasking operations on an enum.

	just add:

	DEFINE_ENUM_FLAG_OPERATOR( my_enum_name )

	...after your declaration to get access to bitmasking functions and comparisons.
*/

#undef DEFINE_ENUM_FLAG_OPERATORS
#define DEFINE_ENUM_FLAG_OPERATOR(ENUMTYPE) \
inline ENUMTYPE operator | (ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) | ((int)b)); } \
inline ENUMTYPE &operator |= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(((int &)a) |= ((int)b)); } \
inline ENUMTYPE operator & (ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) & ((int)b)); } \
inline ENUMTYPE &operator &= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(((int &)a) &= ((int)b)); } \
inline ENUMTYPE operator ~ (ENUMTYPE a) { return ENUMTYPE(~((int)a)); } \
inline ENUMTYPE operator ^ (ENUMTYPE a, ENUMTYPE b) { return ENUMTYPE(((int)a) ^ ((int)b)); } \
inline ENUMTYPE &operator ^= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(((int &)a) ^= ((int)b)); }\
inline bool operator> (ENUMTYPE a, int b) {  return (((int)a) > b); }

/*
	The TMP_VAR macro generates a unique variable name to hold an object
	and manage it's lifetime within a scope.

	variable lives on the stack and is destroyed when the scope is exited.
*/
#define TMP_VAR_HELPER1(x, y) x ## y
#define TMP_VAR_HELPER2(x, y) TMP_VAR_HELPER1(x, y)

#define SCOPED_VAR(code) auto TMP_VAR_HELPER2(_tmp_var_, __LINE__) = code;

#define STRTOL(type, code) static_cast<type>( strtol( code.c_str(), (char**) NULL, 10 ) )
#define STRTOF(type, code) static_cast<type>( strtof( code.c_str(), (char**) NULL ) )

// ----------------------------------------------------------------------------
// game engine specific stuff that doesn't change often
//
// putting things in here will speed up partial compiles
// ----------------------------------------------------------------------------

#include "master_fwd_decl.h"
#include "master_enum.h"

#include "stringutil.h"
#include "logfile.h"

#include "mathutil.h"
#include "parseutil.h"
#include "tokenizer.h"

// ----------------------------------------------------------------------------
// handy macros

#define s_format w_stringutil::format
#define s_commas w_stringutil::format_with_commas
#define log_msg logfile->msg
#define log_warning logfile->warning
#define log_error logfile->error

// leaving room to customize epsilon later if this is too small
constexpr float W_EPSILON = FLT_EPSILON;

bool inline fequals( float a, float b )
{
	return ( std::abs( (double)a - b ) < W_EPSILON );
}

/*
	the size of the virtual screen. all rendering is done on the assumption
	of these dimensions. the renderer handles scaling this up to the actual
	game window the player sees. this makes a lot of things simpler.
*/
constexpr float v_window_w = 16*19;
constexpr float v_window_h = (16*9) + 100;

constexpr float v_window_hw = v_window_w / 2.0f;
constexpr float v_window_hh = v_window_h / 2.0f;

/*
	templated min/max functions
*/

template<typename T> T inline w_min( T a, T b )
{
	if( a < b )
		return a;
	
	return b;
}

template<typename T> T inline w_max( T a, T b )
{
	if( a > b )
	{
		return a;
	}
	
	return b;
}

template<typename T> T inline w_clamp( T v, T min, T max )
{
	return w_max( min, w_min( max, v ) );
}

/*
	useful type definitions to increase code readability
*/

typedef w_vec2 w_uv;
typedef w_vec2 w_sz;
