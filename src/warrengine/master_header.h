
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

	using f_draw_control = std::function<void( UI_Control* control, const Rect& rc_ui )>;
	#define f_decl_draw_control( func_name ) void func_name ( UI_Control* control, const Rect& rc_ui )

	using f_tile_map_spawn_entity = std::function<void( Scene* Scene, const Tile_Map_Asset* tile_map, const Tile_Map_Asset::Chunk* chunk, const Tile_Map_Asset::Tile* tile )>;
	#define f_decl_tile_map_spawn_entity( func_name ) void func_name ( Scene* scene, const Tile_Map_Asset* tile_map, const Tile_Map_Asset::Chunk* chunk, const Tile_Map_Asset::Tile* tile )

	using ec_simple_body_pair = std::pair<Collision_Body_Component*, Collision_Body_Component*>;
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
