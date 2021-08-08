
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_breakout_coll_ball = collision_bits.get();
static const unsigned scene_simple_breakout_coll_paddle = collision_bits.get();
static const unsigned scene_simple_breakout_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

void e_ball::on_collided( simple_collision::pending_collision& coll )
{
	if( coll.entity_b->tag == H( "THE_PADDLE" ) )
	{
		int warren = 5;
	}
}

// ----------------------------------------------------------------------------

void e_paddle::on_collided( simple_collision::pending_collision& coll )
{
	if( coll.entity_b->tag == H( "THE_BALL" ) )
	{
		int warren = 5;
	}
}

// ----------------------------------------------------------------------------

scene_simple_breakout::scene_simple_breakout()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void scene_simple_breakout::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( mouse_mode::os );

	// paddle
	{
		auto e = add_entity<e_paddle>();
		e->tag = H( "THE_PADDLE" );
		e->debug_name = "THE_PADDLE";
		e->set_pos( { 0.f, 0.f } );
		e->simple.friction = 1.0;
		e->simple.type = sc_type::kinematic;
		auto paddle_w = 120.f;
		auto paddle_h = 16.f;
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->rs_opt.color = make_color( color::white, 1.0f );
			ec->add_shape( primitive_shape::filled_rect, rect( -paddle_w / 2.f, -paddle_h / 2.f, paddle_w, paddle_h ) );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->set_as_centered_box( paddle_w, paddle_h );
			ec->set_collision_flags( scene_simple_breakout_coll_paddle, scene_simple_breakout_coll_ball );
		}

		paddle = e;
	}

	// ball
	{
		auto e = add_entity<e_ball>();
		e->tag = H( "THE_BALL" );
		e->debug_name = "THE_BALL";
		e->set_pos( { 0.f, -64.f } );
		e->simple.friction = 0.0;
		e->simple.is_bouncy = true;
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->rs_opt.color = make_color( color::white, 1.0f );
			ec->add_shape( primitive_shape::filled_circle, 12.f );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->set_as_circle( 12.f );
			ec->set_collision_flags( scene_simple_breakout_coll_ball, scene_simple_breakout_coll_geo | scene_simple_breakout_coll_paddle );
		}

		e->apply_impulse( { random::get_random_unit_vector(), 2.5f } );

		ball = e;
	}

	// WORLD GEO

	{
		auto num_primitives = 2;
		auto e = add_entity<entity>();
		e->tag = H( "world_geo" );
		e->simple.type = sc_type::stationary;

		for( int i = 0 ; i < num_primitives ; ++i )
		{
			auto x = random::getf_range( -viewport_hw, viewport_hw );
			auto y = random::getf_range( -viewport_hw, viewport_hw );
			auto w = random::getf_range( 16.f, 80.f );
			auto h = random::getf_range( 16.f, 80.f );

			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_as_centered_box( w, h );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
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

		for( int i = 0 ; i < num_primitives ; ++i )
		{
			auto x = random::getf_range( -viewport_hw, viewport_hw );
			auto y = random::getf_range( -viewport_hw, viewport_hw );
			auto r = random::getf_range( 8.f, 40.f );

			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_as_circle( r );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
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

		// 4 walls
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_breakout_coll_geo, 0 );
		}

		world_geo = e;
	}
}

void scene_simple_breakout::draw()
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

void scene_simple_breakout::draw_ui()
{
	scene::draw_ui();
}

void scene_simple_breakout::update()
{
	scene::update();
}

bool scene_simple_breakout::on_input_pressed( const input_event* evt )
{
	return false;
}

bool scene_simple_breakout::on_input_held( const input_event* evt )
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

bool scene_simple_breakout::on_input_motion( const input_event* evt )
{
	return false;
}
