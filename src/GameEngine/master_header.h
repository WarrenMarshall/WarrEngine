#pragma once

#include "matrix.h"
#include "opengl.h"
#include "shader.h"
#include "render_buffer.h"

#include "i_lifecycle.h"
#include "time.h"
#include "random.h"
#include "tween.h"
#include "timer.h"
#include "keyvalues.h"

#include "i_transform.h"
#include "i_listener.h"
#include "i_speaker.h"
#include "datatypes.h"
#include "i_reloadable.h"
#include "asset.h"
#include "asset_types.h"
#include "input_mgr.h"
#include "asset_definition.h"
#include "asset_definition_file.h"
#include "asset_definition_file_cache.h"
#include "ui_mgr.h"
#include "engine.h"

#include "particle.h"
#include "particle_spawner.h"
#include "particle_emitter.h"
#include "particle_pool.h"

#include "render.h"
#include "window.h"
#include "cache_assets.h"

#include "bounds.h"
#include "component.h"
#include "entity_fx.h"
#include "entity.h"
#include "game_entities.h"

#include "layer.h"
#include "layer_mgr.h"
#include "layer_background.h"
#include "layer_browser.h"
#include "layer_editor.h"
#include "layer_worldviewport.h"

#include "io_zip.h"
#include "filesystem.h"
#include "file_mem.h"

#include "game.h"
#include "g_custom_game.h"

// ----------------------------------------------------------------------------
// globals for all to enjoy - don't judge

extern bool g_allow_hot_reload;

extern std::unique_ptr<w_logfile> logfile;
extern std::unique_ptr<w_engine> engine;
extern std::unique_ptr<g_custom_game> game;
