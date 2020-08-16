
#pragma once

#include "matrix.h"
#include "opengl.h"
#include "shader.h"
#include "render_buffer.h"

#include "i_lifecycle.h"
#include "util_time.h"
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
#include "game_entities.h"

#include "layer.h"
#include "layer_mgr.h"

#include "io_zip.h"
#include "filesystem.h"
#include "file_mem.h"

#include "game.h"

// ----------------------------------------------------------------------------
// globals for all to enjoy - don't judge

extern bool GE_API g_allow_hot_reload;

extern std::unique_ptr<w_logfile> GE_API logfile;
extern std::unique_ptr<w_engine> GE_API engine;
extern GE_API w_game* game;
