
#include "cmd_line.h"
#include "object_pool.h"

#include "file_zip.h"
#include "file_system.h"
#include "file_mem.h"
#include "file_disk.h"

#include "random.h"
#include "datatypes.h"
#include "scoped_vars.h"

#include "bit_flag_generator.h"
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
#include "render_batch.h"

#include "life_cycle_mgr.h"
#include "clock.h"
#include "noise.h"
#include "tween.h"
#include "timer.h"
#include "keyvalues.h"

#include "transform.h"
#include "asset.h"
#include "palette_asset.h"
#include "texture_source_base.h"
#include "texture_source_asset.h"
#include "gradient_source_asset.h"
#include "texture_asset.h"
#include "particle.h"
#include "particle_spawner.h"
#include "particle_pool.h"
#include "particle_emitter.h"
#include "emitter_parameters_asset.h"
#include "font_asset.h"
#include "tile_set_asset.h"
#include "tile_map_asset.h"
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

#include "asset_cache_mgr.h"
#include "window.h"
#include "scene.h"
#include "scene_mgr.h"
#include "render_state.h"
#include "render.h"

#include "physics_contact_listener.h"
#include "physics_query_callbacks.h"
#include "physics_query.h"
#include "physics_debug_draw.h"
#include "physics_pending_collision.h"

#include "engine.h"

#include "fx.h"

#include "entity_component.h"
#include "entity.h"

#include "scene_controller_required.h"
#include "scene_esc_menu.h"
#include "scene_msg_box.h"
#include "scene_post_process.h"

#include "game.h"

namespace war
{

extern std::unique_ptr<engine> g_engine;
extern std::unique_ptr<ui_mgr> g_ui;

}
