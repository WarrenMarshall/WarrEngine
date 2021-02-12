
#pragma once

extern std::unique_ptr<w_logfile> logfile;

// ----------------------------------------------------------------------------
// attempts to make logging things easier
//
// i.e. log( "a log entry" );
// ----------------------------------------------------------------------------

template<typename ...Params>
constexpr void _log_( Params&&... params )
{
	logfile->msg( fmt::format( std::forward<Params>( params )... ) );
}

template<typename ...Params>
constexpr void _verbose_( Params&&... params )
{
	logfile->verbose( fmt::format( std::forward<Params>( params )... ) );
}

template<typename ...Params>
constexpr void _log_error_( Params&&... params )
{
	logfile->error( fmt::format( std::forward<Params>( params )... ) );
}

#define log( fmt, ... ) _log_( "[{}:{}] " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define log_warning( fmt, ... ) _log_( "[{}:{}] WARNING : " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define log_error( fmt, ... ) _log_error_( "[{}:{}] !ERROR! : " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define log_div() _log_( "[{}:{}] ----------------------------------------\n", __FUNCTION__, __LINE__ )
#define log_verbose( fmt, ... ) _verbose_( "[{}:{}] " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )

// ----------------------------------------------------------------------------
// hashes a string at compile time for quicker comparisons. yoinked from:
// https://stackoverflow.com/questions/2111667/compile-time-string-hashing

using hash = unsigned;
constexpr hash hash_none = 0;

// #C++20 - when MSVC fully supports "consteval" use that instead
//          of "constexpr" to make sure this is compile time only
#if 1
	// this hash is super simple (it just sums up all the values of the letters)
	// and it WILL give false positives for strings with transposed characters.
	//
	// i.e. H("warren") == H("warrne")
	//
	// so be careful.

	unsigned constexpr H( char const* str )
	{
		return *str ?
			static_cast<unsigned int>( *str ) + H( str + 1 ) :
			1;
	}

	// this hash is the one copied from the link above but I don't necessarily
	// see how it's any better than my super simple one above. if it turns out
	// that it IS way better, use it instead.

// 	unsigned constexpr H( char const* str )
// 	{
// 		return *str ?
// 			static_cast<unsigned int>( *str ) + 33 * H( str + 1 ) :
// 			5381;
// 	}
#else
	unsigned consteval H( char const* str )
	{
		return *str ?
			static_cast<unsigned int>( *str ) + 33 * H( str + 1 ) :
			5381;
	}
#endif

// ----------------------------------------------------------------------------
// the size of the virtual screen. all rendering is done on the assumption of
// these dimensions. the renderer handles scaling this up to the actual game
// window the player sees. this makes a lot of things simpler.

extern float v_window_w;
extern float v_window_h;

#define v_window_hw (v_window_w * 0.5f)
#define v_window_hh (v_window_h * 0.5f)

extern float ui_window_w;
extern float ui_window_h;

#define ui_window_hw (ui_window_w * 0.5f)
#define ui_window_hh (ui_window_h * 0.5f)

#define ui_window_scale (v_window_w / ui_window_w)

#include "filesystem.h"

#include "bit_flag_generator.h"
#include "matrix.h"
#include "opengl_color_attachment.h"
#include "opengl_framebuffer.h"
#include "opengl.h"
#include "shader.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_array_object.h"
#include "render_batch.h"

#include "i_life_cycle.h"
#include "w_time.h"
#include "random.h"
#include "tween.h"
#include "timer.h"
#include "keyvalues.h"
#include "config.h"

#undef min	// fixes dumb compile problem
#undef max	// fixes dumb compile problem
#include "datatypes.h"
#include "transform.h"
#include "asset.h"
#include "a_palette.h"
#include "a_src_texture.h"
#include "a_texture.h"
#include "a_gradient.h"
#include "a_anim_texture.h"
#include "particle.h"
#include "particle_spawner.h"
#include "particle_emitter.h"
#include "particle_pool.h"
#include "a_emitter_params.h"
#include "a_font.h"
#include "a_cursor.h"
#include "a_9slice_def.h"
#include "a_sound.h"
#include "a_mesh.h"
#include "input_gamepad.h"
#include "input.h"
#include "asset_definition_file.h"
#include "asset_definition_file_cache.h"
#include "ui_imgui_control_data.h"
#include "ui_control.h"
#include "ui_callback.h"
#include "ui_imgui_result.h"
#include "ui_imgui.h"
#include "ui_mgr.h"

extern std::unique_ptr<w_game> base_game;
#include "engine.h"

#include "render_stats.h"
#include "render.h"
#include "window.h"
#include "cache_assets.h"

#include "physics_contact_listener.h"
#include "physics_query_callbacks.h"
#include "physics_query.h"
#include "physics_debug_draw.h"

#include "entity_component.h"
#include "entity.h"

#include "layer.h"
#include "layer_mgr.h"

#include "layer_esc_menu.h"
#include "layer_msg_box.h"

#include "file_mem.h"
#include "file_zip.h"
#include "file_disk.h"

#include "game.h"

// ----------------------------------------------------------------------------
// globals for all to enjoy - don't judge
// ----------------------------------------------------------------------------

extern std::unique_ptr<w_engine> engine;
//extern std::unique_ptr<w_game> base_game;
