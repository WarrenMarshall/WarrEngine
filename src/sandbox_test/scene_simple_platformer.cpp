
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_platformer_coll_mario = collision_bits.get();
static const unsigned scene_simple_platformer_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

constexpr float max_raycast_length = 250.f;

// ----------------------------------------------------------------------------

scene_simple_platformer::scene_simple_platformer()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void scene_simple_platformer::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( mouse_mode::os );

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
			ec->set_collision_flags( scene_simple_platformer_coll_mario, scene_simple_platformer_coll_geo );
		}
		{
			auto ec = e->add_component<ec_scr_push_outside>();
		}
		{
			auto ec = e->add_component<ec_movement_controller>();
		}

		mario = e;
	}

	// HIT MARKER
	{
		auto e = add_entity<entity>();
		e->tag = H( "hit_marker" );
		e->set_pos( { 0.f, 0.f } );
		e->rs_opt.z_bias = zdepth_debug_bias;
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->rs_opt.color = make_color( color::yellow );
		}

		hit_marker = e;

	}

	// WORLD GEO

	{
		auto e = add_entity<entity>();
		e->tag = H( "world_geo" );

		for( int i = 0 ; i < 24 ; ++i )
		{
			auto x = random::getf_range( -viewport_hw, viewport_hw );
			auto y = random::getf_range( -viewport_hw, viewport_hw );
			auto w = random::getf_range( 16.f, 80.f );
			auto h = random::getf_range( 16.f, 80.f );

			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_as_centered_box( w, h );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_platformer_coll_geo, 0 );
			}
			{
				auto ec = e->add_component<ec_primitive_shape>();
				ec->add_shape( primitive_shape::filled_rect, { x - ( w / 2.f ), y - ( h / 2.f ), w, h } );
				ec->rs_opt.color = make_color( pal::darker, 0.5f );
			}
			{
				auto ec = e->add_component<ec_primitive_shape>();
				ec->add_shape( primitive_shape::rect, { x - ( w / 2.f ), y - ( h / 2.f ), w, h } );
				ec->rs_opt.color = make_color( pal::middle );
			}
		}

		for( int i = 0 ; i < 24 ; ++i )
		{
			auto x = random::getf_range( -viewport_hw, viewport_hw );
			auto y = random::getf_range( -viewport_hw, viewport_hw );
			auto r = random::getf_range( 8.f, 40.f );

			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_as_circle( r );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_platformer_coll_geo, 0 );
			}
			{
				auto ec = e->add_component<ec_primitive_shape>();
				ec->add_shape( primitive_shape::filled_circle, r );
				ec->get_transform()->set_pos( { x, y } );
				ec->rs_opt.color = make_color( pal::darker, 0.5f );
			}
			{

				auto ec = e->add_component<ec_primitive_shape>();
				ec->add_shape( primitive_shape::circle, r );
				ec->get_transform()->set_pos( { x, y } );
				ec->rs_opt.color = make_color( pal::middle );
			}
		}

		world_geo = e;
	}
}

void scene_simple_platformer::draw()
{
	{
		scoped_render_state;
		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ),
			rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	render::draw_world_axis();

	if( b_show_ray )
	{
		render::state->color = make_color( color::orange );
		auto start = mario->get_pos();
		render::draw_line( start, start + ( ray_dir * max_raycast_length ) );
	}
}

void scene_simple_platformer::draw_ui()
{
	scene::draw_ui();
	draw_title( "Simple Platformer" );
}

void scene_simple_platformer::update()
{
	scene::update();

	// show the raycast beam if the right stick is being pushed
	b_show_ray = g_engine->input.get_axis_state( input_id::gamepad_right_stick ).get_size_fast() > 0.f;
}

void scene_simple_platformer::reset_collision_trace_results()
{
	hit_marker->get_component<ec_primitive_shape>()->shapes.clear();

	for( auto& iter : world_geo->get_components<ec_simple_collision_body>() )
	{
		iter->rs_opt.color = make_color( color::dark_teal );
	}
}

bool scene_simple_platformer::on_input_pressed( const input_event* evt )
{
	return false;
}

bool scene_simple_platformer::on_input_held( const input_event* evt )
{

/*
	switch( evt->input_id )
	{
		case input_id::key_left:
		{
			mario->add_delta_pos( vec2( fixed_time_step::per_second( -600.f ), 0.f ) );
		}
		break;

		case input_id::key_right:
		{
			mario->add_delta_pos( vec2( fixed_time_step::per_second( 600.f ), 0.f ) );
		}
		break;
	}
*/

	return false;
}

bool scene_simple_platformer::on_input_motion( const input_event* evt )
{
	return false;
}
