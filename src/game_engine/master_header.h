
#pragma once

// ----------------------------------------------------------------------------
/*
	the size of the virtual screen. all rendering is done on the assumption
	of these dimensions. the renderer handles scaling this up to the actual
	game window the player sees. this makes a lot of things simpler.

	make sure the values in preproc.asset_def match these values.
*/
extern float v_window_w;
extern float v_window_h;

#define v_window_hw (v_window_w / 2.0f)
#define v_window_hh (v_window_h / 2.0f)

#include "matrix.h"
#include "opengl_framebuffer.h"
#include "opengl.h"
#include "shader.h"
#include "render_buffer.h"

#include "i_lifecycle.h"
#include "util_time.h"
#include "random.h"
#include "tween.h"
#include "timer.h"
#include "keyvalues.h"
#include "config.h"

#include "i_transform.h"
#include "i_listener.h"
#include "i_speaker.h"
#include "datatypes.h"
#include "asset.h"
#include "asset_types.h"
#include "input_game_controller.h"
#include "input.h"
#include "asset_definition_file.h"
#include "asset_definition_file_cache.h"
#include "ui_theme.h"
#include "ui_mgr.h"
#include "engine.h"

#include "particle.h"
#include "particle_spawner.h"
#include "particle_emitter.h"
#include "particle_pool.h"

#include "render.h"
#include "window.h"
#include "cache_assets.h"

#include "entity_component.h"
#include "entity.h"

#include "layer.h"
#include "layer_mgr.h"

#include "filesystem.h"
#include "file_mem.h"
#include "file_zip.h"
#include "file_disk.h"

#include "game.h"

// ----------------------------------------------------------------------------
// globals for all to enjoy - don't judge

extern std::unique_ptr<w_logfile> logfile;
extern std::unique_ptr<w_engine> engine;
extern w_game* game;
