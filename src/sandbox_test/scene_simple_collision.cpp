
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_coll_mario = ++collision_bits;
static const unsigned scene_simple_coll_geo = ++collision_bits;

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
		//e->debug_name = "MARIO";
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
			ec->set_collision_flags( scene_simple_coll_mario, scene_simple_coll_geo );
			ec->rs_opt.color = make_color( color::light_blue );
		}

		mario = e;
	}

	// generate random things for mario to collide with

	{
		auto e = add_entity<entity>();
		e->tag = H( "world_geo" );
		//e->debug_name = "WORLD_GEO";

		for( int x = 0 ; x < 24 ; ++x )
		{
			{
				auto ec = e->add_component<simple_collision_component>();
				ec->set_as_centered_box( random::getf_range( 16.f, 80.f ), random::getf_range( 16.f, 80.f ) );
				ec->get_transform()->set_pos( { random::getf_range( -viewport_hw, viewport_hw ), random::getf_range( -viewport_hh, viewport_hh ) } );
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
				ec->rs_opt.color = make_color( color::dark_teal );
			}
		}
	}

	{
		auto e = add_entity<entity>();
		e->tag = H( "world_geo2" );
		//e->debug_name = "WORLD_GEO2";

		for( int x = 0 ; x < 24 ; ++x )
		{
			{
				auto ec = e->add_component<simple_collision_component>();
				ec->set_as_circle( random::getf_range( 8.f, 40.f ) );
				ec->get_transform()->set_pos( { random::getf_range( -viewport_hw, viewport_hw ), random::getf_range( -viewport_hh, viewport_hh ) } );
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
				ec->rs_opt.color = make_color( color::teal );
			}
		}
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
			return true;
		}
		break;
	}

	return false;
}
