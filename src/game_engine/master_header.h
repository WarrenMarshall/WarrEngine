
#pragma once

extern std::unique_ptr<w_logfile> logfile;

// ----------------------------------------------------------------------------
/*
	the size of the virtual screen. all rendering is done on the assumption
	of these dimensions. the renderer handles scaling this up to the actual
	game window the player sees. this makes a lot of things simpler.
*/
extern float v_window_w;
extern float v_window_h;

#define v_window_hw (v_window_w * 0.5f)
#define v_window_hh (v_window_h * 0.5f)

#include "bit_flag_generator.h"
#include "matrix.h"
#include "opengl_framebuffer.h"
#include "opengl.h"
#include "shader.h"
#include "render_buffer.h"

#include "i_life_cycle.h"
#include "util_time.h"
#include "random.h"
#include "tween.h"
#include "timer.h"
#include "keyvalues.h"
#include "config.h"

#include "i_transform.h"
#include "i_listener.h"
#include "i_speaker.h"
#undef min	// fixes dumb compile problem
#undef max	// fixes dumb compile problem
#include "datatypes.h"
#include "asset.h"
#include "a_texture.h"
#include "a_subtexture.h"
#include "a_gradient.h"
#include "a_anim_texture.h"
#include "a_emitter_params.h"
#include "a_font.h"
#include "a_cursor.h"
#include "a_9slice_def.h"
#include "a_sound.h"
#include "input_game_controller.h"
#include "input.h"
#include "asset_definition_file.h"
#include "asset_definition_file_cache.h"
#include "ui_theme.h"
#include "ui_imgui.h"
#include "ui_mgr.h"
#include "engine.h"

#include "particle.h"
#include "particle_spawner.h"
#include "particle_emitter.h"
#include "particle_pool.h"

#include "render.h"
#include "window.h"
#include "cache_assets.h"

#include "physics_callbacks.h"
#include "physics_debug_draw.h"
#include "physics.h"

#include "entity_component.h"
#include "entity.h"

#include "layer.h"
#include "layer_mgr.h"

#include "layer_esc_menu.h"

#include "filesystem.h"
#include "file_mem.h"
#include "file_zip.h"
#include "file_disk.h"

#include "game.h"

// ----------------------------------------------------------------------------
// globals for all to enjoy - don't judge

extern std::unique_ptr<w_engine> engine;
extern std::unique_ptr<w_game> base_game;
