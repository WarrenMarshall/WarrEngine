
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_coll_mario = ++collision_bits;
static const unsigned scene_simple_coll_skull = ++collision_bits;

// ----------------------------------------------------------------------------

scene_simple_collision::scene_simple_collision()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = true;
	flags.is_debug_physics_scene = true;
}

void scene_simple_collision::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// MARIO
	{
		auto e = add_entity<entity>();
		e->tag = H( "mario" );
	#ifdef _DEBUG
		e->debug_name = "MARIO";
	#endif
		e->transform_set_pos( { -80.f, 0.f } );
		{
			auto ec = e->add_component<sprite_component>();
			ec->rs_opt.color = make_color( color::white, 0.25f );
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<simple_collision_component>();
			//ec->set_as_centered_box( 32.f, 32.f );
			ec->set_as_circle( 16.f );
			ec->set_collision_flags( scene_simple_coll_mario, scene_simple_coll_skull );
		}

		mario = e;
	}

	// SKULL
	{
		auto e = add_entity<entity>();
		e->tag = H( "skull" );
	#ifdef _DEBUG
		e->debug_name = "SKULL";
	#endif
		e->transform_set_pos( { 80.f, 0.f } );
		{
			auto ec = e->add_component<sprite_component>();
			ec->rs_opt.color = make_color( color::white, 0.25f );
			ec->init( "tex_skull" );
		}
		{
			auto ec = e->add_component<simple_collision_component>();
			ec->set_as_centered_box( 48.f, 48.f );
			ec->set_collision_flags( scene_simple_coll_skull, 0 );
		}

		skull = e;
	}
}

void scene_simple_collision::draw()
{
	{
		scoped_render_state;
		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ), rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	render::draw_world_axis();
}

void scene_simple_collision::draw_ui()
{
	scene::draw_ui();
	draw_title( "Simple Collisions" );
}

bool scene_simple_collision::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			mario->transform_delta_pos( evt->delta * 150.f * fixed_time_step::per_second_scaler );
			return true;
		}
		break;

		case input_id::gamepad_right_stick:
		{
			skull->transform_delta_pos( evt->delta * 150.f * fixed_time_step::per_second_scaler );
			return true;
		}
		break;
	}

	return false;
}
