
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_space_coll_mario = collision_bits.get();
static const unsigned scene_simple_space_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

scene_simple_space::scene_simple_space()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void scene_simple_space::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// MARIO
	{
		auto radius = 12.f;
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
			ec->set_as_circle( radius );
			ec->set_collision_flags( scene_simple_space_coll_mario, scene_simple_space_coll_geo );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->rs_opt.color = make_color( color::green, 0.25f );
			ec->add_shape( primitive_shape::filled_circle, radius );
		}
		{
			auto ec = e->add_component<ec_scr_bounce_off>();
		}
		{
			auto ec = e->add_component<ec_movement_controller>();
			ec->set_damping( damping::outer_space );
		}

		mario = e;
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
				ec->set_collision_flags( scene_simple_space_coll_geo, 0 );
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
				ec->set_collision_flags( scene_simple_space_coll_geo, 0 );
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

void scene_simple_space::draw()
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

void scene_simple_space::draw_ui()
{
	scene::draw_ui();
	draw_title( "Simple Space Drifter" );
}

void scene_simple_space::update()
{
	scene::update();

	// wrap player around the edges of the screen

	auto pos = mario->get_pos();

	if( pos.x < -viewport_hw )		pos.x = viewport_hw;
	if( pos.x > viewport_hw )		pos.x = -viewport_hw;
	if( pos.y < -viewport_hh )		pos.y = viewport_hh;
	if( pos.y > viewport_hh )		pos.y = -viewport_hh;

	mario->set_pos( pos );
}

bool scene_simple_space::on_input_pressed( const input_event* evt )
{
/*
	switch( evt->input_id )
	{
	}
*/

	return false;
}

bool scene_simple_space::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			auto force = 0.05f;
			vec2 delta = evt->delta;

			mario->add_force( evt->delta * force );

			return true;
		}
		break;
	}

	return false;
}
