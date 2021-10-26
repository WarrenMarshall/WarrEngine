
#include <warrengine.h>

using namespace war;

// ----------------------------------------------------------------------------

static void draw_tiled_background()
{
	scoped_render_state;

	Render::state->color = make_color( e_pal::darker );
	Render::draw_tiled( g_engine->find_asset<Texture_Asset>( "engine_tile_background_stripe" ), Rect( -viewport_w, -viewport_h, viewport_w * 2.f, viewport_h * 2.f ) );
}

static void draw_gradient_background()
{
	scoped_render_state;

	Render::draw_quad( g_engine->find_asset<Texture_Asset>( "background_gradient" ), Rect( 0.f, 0.f, viewport_w, viewport_h ) );
}

static void draw_title( const std::string& title )
{
	scoped_render_state;

	Render::state->align = e_align::centered;
	Render::state->color = make_color( e_pal::lightest );
	Render::state->scale = 2.f;

	Render::draw_string( title, { ui_w / 2.f, 16.f } );
}

// ----------------------------------------------------------------------------

#include "sandbox_test.h"

#include "scene_palette.h"
#include "scene_angle_dirs.h"
#include "scene_coords.h"
#include "scene_ui.h"
#include "scene_textures.h"
#include "scene_particles.h"
#include "scene_constraints.h"
#include "scene_spatial.h"
#include "scene_cameras.h"
#include "scene_meshes.h"
#include "scene_entity_picking.h"
#include "scene_sound_music.h"
#include "scene_tweens_timers.h"
#include "scene_box2d.h"
#include "scene_sensors.h"
#include "scene_interact.h"
#include "scene_top_down.h"
#include "scene_bounce.h"
#include "scene_breakout.h"
#include "scene_platformer.h"
#include "scene_controller.h"
#include "scene_tile_map.h"
#include "scene_wrapped_text.h"
#include "scene_noise.h"

#include "scene_main_menu.h"
