
#include "cache.h"

#include "cmd_line.h"
#include "object_pool.h"

#include "file_zip.h"
#include "file_system.h"
#include "file_mem.h"
#include "file_disk.h"

#include "life_cycle_mgr.h"
#include "random.h"
#include "vec2.h"
#include "vec3.h"
#include "rect.h"
#include "bounding_box.h"
#include "range.h"
#include "color.h"
#include "noise.h"
#include "timeline_key_frame.h"
#include "timeline.h"
#include "timeline_nodes_key_frame.h"
#include "timeline_nodes.h"
#include "scoped_vars.h"
#include "util_geo.h"

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
#include "primitive_batch.h"

#include "clock.h"
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

namespace war
{
	// ----------------------------------------------------------------------------
	// callback function typedefs and declaration macros

	using f_draw_control = std::function<void( ui_control* control, const rect& rc_ui )>;
	#define f_decl_draw_control( func_name ) void func_name ( ui_control* control, const rect& rc_ui )

	using f_tile_map_spawn_entity = std::function<void( scene* scene, tile_map_asset::tile* tile )>;
	#define f_decl_tile_map_spawn_entity( func_name ) void func_name ( scene* scene, tile_map_asset::tile* tile )

	using ec_simple_body_pair = std::pair<ec_simple_collision_body*, ec_simple_collision_body*>;
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

#include "simple_collision_query_callbacks.h"
#include "simple_collision_query.h"
#include "simple_collision_pending_collision.h"
#include "simple_collision_world.h"

#include "asset_cache_mgr.h"
#include "window.h"
#include "scene.h"
#include "scene_mgr.h"
#include "render_state.h"
#include "render.h"

#include "box2d_physics_contact_listener.h"
#include "box2d_physics_query_callbacks.h"
#include "box2d_physics_query.h"
#include "box2d_physics_debug_draw.h"
#include "box2d_physics_pending_collision.h"

#include "engine.h"

#include "entity_component.h"
#include "entity.h"

#include "scene_controller_required.h"
#include "scene_esc_menu.h"
#include "scene_msg_box.h"
#include "scene_post_process.h"

#include "game.h"

namespace war
{

[[nodiscard]] float lerp( float a, float b, float lerp_factor );
[[nodiscard]] vec2 lerp( vec2 a, vec2 b, float lerp_factor );

extern std::unique_ptr<engine> g_engine;
extern std::unique_ptr<ui_mgr> g_ui;

}
