
#include <warrengine.h>

using namespace war;

// ----------------------------------------------------------------------------

static void draw_tiled_background()
{
	scoped_render_state;

	render::state->color = make_color( pal::darker );
	render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ), rect( -viewport_w, -viewport_h, viewport_w * 2.f, viewport_h * 2.f ) );
}

static void draw_gradient_background()
{
	scoped_render_state;

	render::draw_quad( g_engine->find_asset<texture_asset>( "background_gradient" ), rect( 0.f, 0.f, viewport_w, viewport_h ) );
}

static void draw_title( std::string_view title )
{
	scoped_render_state;

	render::state->align = align::centered;
	render::state->color = make_color( pal::brightest );
	render::state->scale = 2.f;

	render::draw_string( title, { ui_w / 2.f, 16.f } );
}

// ----------------------------------------------------------------------------

#include "sandbox_test.h"

#include "scene_box2d_entities.h"

#include "scene_palette.h"
#include "scene_coords.h"
#include "scene_ui.h"
#include "scene_textures.h"
#include "scene_particles.h"
#include "scene_meshes.h"
#include "scene_entity_picking.h"
#include "scene_sound_board.h"
#include "scene_tweens.h"
#include "scene_box2d.h"
#include "scene_simple_collision.h"
#include "scene_controller.h"
#include "scene_tilemap.h"
#include "scene_wrapped_text.h"
#include "scene_noise.h"

#include "scene_main_menu.h"
