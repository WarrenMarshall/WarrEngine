
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_coll_player = collision_bits.get();
static const unsigned scene_simple_coll_geo = collision_bits.next();
static const unsigned scene_simple_coll_npc = collision_bits.next();

// ----------------------------------------------------------------------------

constexpr float max_raycast_length = 350.f;
constexpr auto player_radius = 12.f;

// ----------------------------------------------------------------------------

scene_simple_collision::scene_simple_collision()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

entity* scene_simple_collision::spawn_player()
{
	auto e = add_entity<entity>();
	e->set_pos( { 0.f, 0.f } );
	e->set_scale( 1.25f );
	e->set_mc_friction( 0.30f );
	//e->mc_affected_by_gravity = true;
	{
		auto ec = e->add_component<ec_sprite>();
		ec->rs_opt.color = make_color( color::white, 1.f );
		ec->init( "anim_player_run" );
	}
	{
		auto ec = e->add_component<ec_simple_collision_body>();
		ec->set_as_centered_box( player_radius * 2.f, player_radius * 2.f );
		//ec->set_as_circle( player_radius );
		ec->set_collision_flags( scene_simple_coll_player, scene_simple_coll_geo | scene_simple_coll_player );
	}
	{
		auto ec = e->add_component<ec_primitive_shape>();
		ec->rs_opt.color = make_color( color::green, 0.25f );
		ec->add_shape( primitive_shape::filled_circle, player_radius );
	}

	first_player = false;

	return e;
}


void scene_simple_collision::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( mouse_mode::os );

	// MARIO
	player = spawn_player();
	player->add_delta_pos( { -40.f, 0.f } );
	spawn_player();

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
		int num_primitives = 30;

		auto e = add_entity<entity>();
		e->tag = H( "world_geo" );
		e->sc_type = sc_type::stationary;

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

		for( int i = 0 ; i < num_primitives ; ++i )
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
		auto start = player->get_pos();
		render::draw_line( start, start + ( ray_dir * max_raycast_length ) );
	}
}

void scene_simple_collision::draw_ui()
{
	scene::draw_ui();
	//draw_title( "Simple Collisions" );

	render::draw_string( std::format( "Velocity : {:.1f}, {:.1f}",
		player->velocity.x, player->velocity.y ), vec2( 8.f, 24.f ) );
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
		case input_id::key_1:
		{
			auto ec = player->get_component<ec_simple_collision_body>();
			ec->set_as_centered_box( player_radius * 2.f, player_radius * 2.f );
		}
		break;

		case input_id::key_2:
		{
			auto ec = player->get_component<ec_simple_collision_body>();
			ec->set_as_circle( player_radius );
		}
		break;

		case input_id::key_3:
		{
			auto ec = player->get_component<ec_simple_collision_body>();
			ec->set_as_polygon(
				{
					{ -player_radius, -player_radius },
					{ player_radius, -player_radius },
					{ player_radius, player_radius },
					{ -player_radius, player_radius }
				}
			);
		}
		break;

		case input_id::gamepad_button_y:
		{
			spawn_player();
		}
		break;

		case input_id::mouse_button_right:
		{
			auto pos = coord_system::window_to_world_pos( evt->mouse_pos );
			player->set_pos( pos );
		}
		break;

		// ALL

		case input_id::gamepad_button_left_shoulder:
		case input_id::key_a:
		{
			reset_collision_trace_results();

			auto start = player->get_pos();
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::raycast_all callback;
			sc_world->ray_cast( &callback, player, start, end );

			if( callback.hit_something )
			{
				auto ec = hit_marker->get_component<ec_primitive_shape>();
				ec->shapes.clear();

				for( auto& hit : callback.results )
				{
					ec->add_shape( primitive_shape::rect, rect::create_centered( 6.f ), hit.pos );
					hit.scc->rs_opt.color = make_color( color::teal );
					hit.scc->rs_opt.glow = 2.f;
				}
			}
		}
		break;

		// CLOSEST

		case input_id::gamepad_button_right_shoulder:
		case input_id::key_c:
		{
			reset_collision_trace_results();

			auto start = player->get_pos();
			auto end = start + ( ray_dir * max_raycast_length );

			simple_collision::raycast_closest callback;
			sc_world->ray_cast( &callback, player, start, end );

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
			float force = 50.f;

			player->add_force_x( evt->delta.x * fixed_time_step::per_second( force ) );
			player->add_force_y( evt->delta.y * -fixed_time_step::per_second( force ) );

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
