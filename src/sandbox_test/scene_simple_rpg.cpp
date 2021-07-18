
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_rpg_coll_mario = collision_bits.get();
static const unsigned scene_simple_rpg_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

scene_simple_rpg::scene_simple_rpg()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

// 1. support horizontal and vertical offsets from tiled
// 2. spawn player entity as rpg hero dude

void scene_simple_rpg::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// world

	{
		world = add_entity<entity>();

		{
			auto ec = world->add_component<ec_tile_map>();
			//ec->set_collision_flags( coll_world, 0 );
			ec->init( "ts_ultima", "tm_simple_rpg" );
			//ec->spawn_entities( this, spawn_entity );
		}
	}

	// MARIO
	{
		auto e = add_entity<entity>();
		e->tag = H( "mario" );
		e->set_pos( { -80.f, 0.f } );
		{
			auto ec = e->add_component<ec_sprite>();
			ec->rs_opt.color = make_color( color::white, 1.f );
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			//ec->set_as_centered_box( 24.f, 24.f );
			ec->set_as_circle( 12.f );
			ec->set_collision_flags( scene_simple_rpg_coll_mario, scene_simple_rpg_coll_geo );
		}
		{
			auto ec = e->add_component<ec_scr_push_outside>();
		}
		{
			auto ec = e->add_component<ec_movement_controller>();
		}

		mario = e;
	}
}

void scene_simple_rpg::draw()
{
	{
		scoped_render_state;
		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ),
			rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	render::draw_world_axis();

}

void scene_simple_rpg::draw_ui()
{
	scene::draw_ui();
	draw_title( "Simple RPG" );
}

void scene_simple_rpg::update()
{
	scene::update();

}

bool scene_simple_rpg::on_input_pressed( const input_event* evt )
{

	return false;
}

bool scene_simple_rpg::on_input_held( const input_event* evt )
{

	return false;
}

bool scene_simple_rpg::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			float force = 2.0f;
			vec2 delta = evt->delta;

			mario->set_force( evt->delta * 2.0f );

			return true;
		}
		break;
	}

	return false;
}
