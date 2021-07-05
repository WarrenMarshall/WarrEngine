
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_coll_mario = collision_bits.get();
static const unsigned scene_simple_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

constexpr float max_raycast_length = 250.f;

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
		auto e = add_entity<entity>();
		e->tag = H( "mario" );
		e->set_pos( { -80.f, 0.f } );
		{
			auto ec = e->add_component<sprite_component>();
			ec->rs_opt.color = make_color( color::white, 1.f );
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<simple_collision_component>();
			//ec->set_as_centered_box( 24.f, 24.f );
			ec->set_as_circle( 12.f );
			ec->set_collision_flags( scene_simple_coll_mario, scene_simple_coll_geo );
			ec->rs_opt.color = make_color( color::orange );
		}

		mario = e;
	}

	// HIT MARKER
	{
		auto e = add_entity<entity>();
		e->tag = H( "hit_marker" );
		e->set_pos( { 0.f, 0.f } );
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->rs_opt.color = make_color( color::yellow );
		}

		hit_marker = e;

	}

	// static geo

	{
		auto e = add_entity<entity>();
		e->tag = H( "world_geo" );

		for( int x = 0 ; x < 24 ; ++x )
		{
			{
				auto ec = e->add_component<simple_collision_component>();
				ec->set_as_centered_box( random::getf_range( 16.f, 80.f ), random::getf_range( 16.f, 80.f ) );
				ec->get_transform()->set_pos(
					{
						random::getf_range( -viewport_hw, viewport_hw ),
						random::getf_range( -viewport_hh, viewport_hh )
					}
				);
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
				ec->rs_opt.color = make_color( color::dark_teal );
			}
		}

		for( int x = 0 ; x < 24 ; ++x )
		{
			{
				auto ec = e->add_component<simple_collision_component>();
				ec->set_as_circle( random::getf_range( 8.f, 40.f ) );
				ec->get_transform()->set_pos(
					{
						random::getf_range( -viewport_hw, viewport_hw ),
						random::getf_range( -viewport_hh, viewport_hh )
					}
				);
				ec->set_collision_flags( scene_simple_coll_geo, 0 );
				ec->rs_opt.color = make_color( color::dark_teal );
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
		render::state->color = make_color( color::orange );
		auto start = mario->get_transform()->pos;
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
	hit_marker->get_component<primitive_shape_component>()->shapes.clear();

	for( auto& iter : world_geo->get_components<simple_collision_component>() )
	{
		iter->rs_opt.color = make_color( color::dark_teal );
	}
}

bool scene_simple_collision::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		// QUICK

		case input_id::gamepad_button_dpad_up:
		case input_id::key_q:
		{
			reset_collision_trace_results();

			auto start = mario->get_transform()->pos;
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::raycast_quick callback;
			g_engine->simple_collision.world->ray_cast( &callback, mario, start, end );

			if( callback.hit_something )
			{
				log( "WE HIT SOMETHING" );
			}
		}
		break;

		// ALL

		case input_id::gamepad_button_dpad_down:
		case input_id::key_a:
		{
			reset_collision_trace_results();

			auto start = mario->get_transform()->pos;
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::raycast_all callback;
			g_engine->simple_collision.world->ray_cast( &callback, mario, start, end );

			if( callback.hit_something )
			{
				auto ec = hit_marker->get_component<primitive_shape_component>();
				ec->shapes.clear();

				for( auto& hit : callback.results )
				{
					ec->add_shape( primitive_shape::rect, rect::create_centered( 6.f ), hit.pos );
					hit.scc->rs_opt.color = make_color( color::teal );
				}
			}
		}
		break;

		// CLOSEST

		case input_id::gamepad_button_dpad_left:
		case input_id::key_c:
		{
			reset_collision_trace_results();

			auto start = mario->get_transform()->pos;
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::raycast_closest callback;
			g_engine->simple_collision.world->ray_cast( &callback, mario, start, end );

			if( callback.hit_something )
			{
				auto ec = hit_marker->get_component<primitive_shape_component>();
				ec->shapes.clear();
				ec->add_shape( primitive_shape::rect, rect::create_centered( 6.f ), callback.result.pos );

				callback.result.scc->rs_opt.color = make_color( color::teal );
			}
		}
		break;

		case input_id::gamepad_button_y:
		{
			process_simple_collisions();
		}
		break;
	}

	return false;
}

bool scene_simple_collision::on_input_held( const input_event* evt )
{

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

	return false;
}

bool scene_simple_collision::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			mario->add_linear_force( evt->delta * fixed_time_step::per_second( 150.f ) );

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
