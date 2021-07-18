
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_coll_mario = collision_bits.get();
static const unsigned scene_simple_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

constexpr float max_raycast_length = 350.f;

// ----------------------------------------------------------------------------

scene_simple_collision::scene_simple_collision()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void scene_simple_collision::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// MARIO
	{
		auto radius = 12.f;
		auto e = add_entity<entity>();
		e->tag = H( "mario" );
		e->set_pos( { -80.f, 0.f } );
		e->set_scale( 1.5f );
		{
			auto ec = e->add_component<ec_sprite>();
			ec->rs_opt.color = make_color( color::white, 1.f );
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			//ec->set_as_centered_box( radius * 2.f, radius * 2.f );
			ec->set_as_circle( radius );
			ec->set_collision_flags( scene_simple_coll_mario, scene_simple_coll_geo );
		}
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->rs_opt.color = make_color( color::green, 0.25f );
			ec->add_shape( primitive_shape::filled_circle, radius );
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
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
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
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
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

void scene_simple_collision::draw()
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
		render::state->color = make_color( color::green, 0.25f );
		auto start = mario->get_pos();
		render::draw_line( start, start + ( ray_dir * max_raycast_length ) );
	}
}

void scene_simple_collision::draw_ui()
{
	scene::draw_ui();
	draw_title( "Simple Collisions" );
}

void scene_simple_collision::update()
{
	scene::update();

	// show the raycast beam if the right stick is being pushed
	b_show_ray = g_engine->input.get_axis_state( input_id::gamepad_right_stick ).get_size_fast() > 0.f;
}

void scene_simple_collision::reset_collision_trace_results()
{
	hit_marker->get_component<ec_primitive_shape>()->shapes.clear();

	for( auto& iter : world_geo->get_components<ec_simple_collision_body>() )
	{
		iter->rs_opt.color = make_color( color::dark_teal );
	}
}

bool scene_simple_collision::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse_button_right:
		{
			auto pos = coord_system::window_to_world_pos( evt->mouse_pos );
			mario->set_pos( pos );
		}
		break;

		// ALL

		case input_id::gamepad_button_left_shoulder:
		case input_id::key_a:
		{
			reset_collision_trace_results();

			auto start = mario->get_pos();
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::raycast_all callback;
			g_engine->simple_collision.world->ray_cast( &callback, mario, start, end );

			if( callback.hit_something )
			{
				auto ec = hit_marker->get_component<ec_primitive_shape>();
				ec->shapes.clear();

				for( auto& hit : callback.results )
				{
					ec->add_shape( primitive_shape::rect, rect::create_centered( 6.f ), hit.pos );
					hit.scc->rs_opt.color = make_color( color::teal );
					hit.scc->rs_opt.glow = 2.0f;
				}
			}
		}
		break;

		// CLOSEST

		case input_id::gamepad_button_right_shoulder:
		case input_id::key_c:
		{
			reset_collision_trace_results();

			auto start = mario->get_pos();
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::raycast_closest callback;
			g_engine->simple_collision.world->ray_cast( &callback, mario, start, end );

			if( callback.hit_something )
			{
				auto ec = hit_marker->get_component<ec_primitive_shape>();
				ec->shapes.clear();
				ec->add_shape( primitive_shape::rect, rect::create_centered( 6.f ), callback.result.pos );

				callback.result.scc->rs_opt.color = make_color( color::teal );
			}
		}
		break;
	}

	return false;
}

bool scene_simple_collision::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			float force = 2.0f;
			vec2 delta = evt->delta;

			mario->set_force( evt->delta * force );

			return true;
		}
		break;

		case input_id::gamepad_right_stick:
		{
			ray_dir = evt->delta;
			return true;
		}
		break;
	}

	return false;
}
