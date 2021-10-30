

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
// collision constants and helpers
// ----------------------------------------------------------------------------

constexpr auto collision_gravity_default = 9.81f;
constexpr auto c2_world_scale_factor = 100.f;

[[nodiscard]] constexpr f32 to_c2( f32 v )
{
	return ( v / c2_world_scale_factor );
}

[[nodiscard]] constexpr f32 from_c2( f32 v )
{
	return ( v * c2_world_scale_factor );
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

#include "cache.h"

#include "cmd_line.h"
#include "object_pool.h"

#include "zip_file.h"
#include "file_system.h"
#include "mem_file.h"
#include "disk_file.h"

#include "life_cycle.h"
#include "random.h"
#include "vec2.h"
#include "vec3.h"
#include "rect.h"
#include "quad_tree.h"
#include "bounding_box.h"
#include "range.h"
#include "color.h"
#include "noise.h"
#include "timeline_of_values.h"
#include "timeline_of_nodes.h"
#include "scoped_vars.h"

#include "coord.h"
#include "matrix.h"
#include "opengl_color_attachment.h"
#include "opengl_framebuffer.h"
#include "opengl.h"
#include "shader.h"
#include "render_vertex.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_array_object.h"
#include "primitive_batch.h"

#include "clock.h"
#include "tween.h"
#include "timer.h"
#include "keyvalues.h"

#include "transform.h"
#include "asset.h"
#include "palette_asset.h"
#include "texture_source_asset.h"
#include "texture_asset.h"
#include "particle.h"
#include "particle_spawner.h"
#include "particle_pool.h"
#include "particle_emitter.h"
#include "emitter_parameters_asset.h"
#include "font_asset.h"
#include "tile_set_asset.h"
#include "tile_map_asset.h"

namespace war
{
	// ----------------------------------------------------------------------------
	// callback function typedefs and declaration macros

	using f_draw_control = std::function<void( UI_Control* control, const Rect& rc_ui )>;
	#define f_decl_draw_control( func_name ) void func_name ( UI_Control* control, const Rect& rc_ui )

	using f_tile_map_spawn_entity = std::function<void( Scene* Scene, const Tile_Map_Asset* tile_map, const Tile_Map_Asset::Chunk* chunk, const Tile_Map_Asset::Tile* tile )>;
	#define f_decl_tile_map_spawn_entity( func_name ) void func_name ( Scene* scene, const Tile_Map_Asset* tile_map, const Tile_Map_Asset::Chunk* chunk, const Tile_Map_Asset::Tile* tile )

	using ec_collision_body_pair = std::pair<Collision_Body_Component*, Collision_Body_Component*>;
}

#include "cursor_asset.h"
#include "slice_def_asset.h"
#include "sound_asset.h"
#include "mesh_asset.h"
#include "input_gamepad.h"
#include "input.h"
#include "asset_file_definition.h"
#include "ui_control_data.h"
#include "ui_control.h"
#include "ui_callback.h"
#include "ui_result.h"
#include "ui_mgr.h"
#include "render_stats.h"

#include "collision_query_callbacks.h"
#include "collision_pending_collision.h"
#include "collision_world.h"

#include "asset_cache_mgr.h"
#include "window.h"
#include "scene.h"
#include "scene_mgr.h"
#include "render_state.h"
#include "render.h"

#include "box2d_contact_listener.h"
#include "box2d_query_callbacks.h"
#include "box2d_query.h"
#include "box2d_debug_draw.h"
#include "box2d_pending_collision.h"

#include "engine.h"

#include "entity_component.h"
#include "entity.h"

#include "scene_controller_required.h"
#include "scene_esc_menu.h"
#include "scene_msg_box.h"
#include "scene_post_process.h"
#include "scene_transient.h"

#include "game.h"

namespace war
{

[[nodiscard]] f32 lerp( f32 a, f32 b, f32 lerp_factor );
[[nodiscard]] Vec2 lerp( Vec2 a, Vec2 b, f32 lerp_factor );

extern std::unique_ptr<Engine> g_engine;
extern std::unique_ptr<UI_Mgr> g_ui;

}
