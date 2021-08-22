
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_top_down_player = collision_bits.get();
static const unsigned scene_simple_top_down_geo = collision_bits.next();
static const unsigned scene_simple_top_down_npc = collision_bits.next();

constexpr auto player_collision_radius = 8.f;

// ----------------------------------------------------------------------------

constexpr float max_raycast_length = 350.f;
constexpr auto radius = 12.f;

// ----------------------------------------------------------------------------

scene_simple_top_down::scene_simple_top_down()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

f_decl_tile_map_spawn_entity( topdown_spawn_entity )
{
	auto gameplay_scene = (scene_simple_top_down*)scene;

	switch( tile->idx )
	{
		case 4:
		{
			auto tmc = gameplay_scene->world->get_component<ec_tile_map>();

			auto e = scene->add_entity<entity>();
			e->set_pos( vec2( tile->x_idx * tmc->tile_map->tile_sz, tile->y_idx * tmc->tile_map->tile_sz ) );
			e->add_delta_pos( vec2( tmc->tile_map->tile_sz / 2.f, tmc->tile_map->tile_sz / 2.f ) );
			e->simple.is_affected_by_gravity = true;
			e->simple.friction = 0.1f;

			{
				auto ec = e->add_component<ec_primitive_shape>();
				ec->add_shape( primitive_shape::point );
			}
			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->tag = H( "player_body" );
				ec->set_as_circle( player_collision_radius );

				ec->set_collision_flags( scene_simple_top_down_player, scene_simple_top_down_geo );
			}
			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->tag = H( "ground_sensor" );
				ec->set_body_collider_type( sc_body_collider_type::sensor );
				ec->set_as_circle( 4.f );
				ec->get_transform()->set_pos( { 0.f, 8.f } );

				ec->set_collision_flags( scene_simple_top_down_player, scene_simple_top_down_geo );
			}
			{
				auto ec = e->add_component<ec_emitter>();
				ec->init( "em_stars" );
				ec->get_transform()->set_scale( 0.25f );
			}

			gameplay_scene->player = e;

			tile->idx = tile_map_asset::tile::empty;
		}
		break;
	}
}

void scene_simple_top_down::pushed()
{
	scene::pushed();

	g_engine->renderer.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( mouse_mode::os );

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

	// GEO

	{
		world = add_entity<entity>( "world" );
		world->simple.type = sc_type::stationary;

		{
			auto ec = world->add_component<ec_tile_map>();
			ec->set_collision_flags( scene_simple_top_down_geo, 0 );
			ec->init( "ts_top_down", "tm_top_down" );
			ec->spawn_entities( this, topdown_spawn_entity );
		}
	}
}

void scene_simple_top_down::draw()
{
	{
		scoped_render_state;
		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ),
			rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	//render::draw_world_axis();

	if( b_show_ray )
	{
		render::state->color = make_color( color::green, 0.25f );
		auto start = player->get_pos();
		render::draw_line( start, start + ( ray_dir * max_raycast_length ) );
	}
}

void scene_simple_top_down::draw_ui()
{
	scene::draw_ui();
	//draw_title( "Simple Collisions" );

	//render::draw_string( std::format( "Velocity : {:.1f}, {:.1f}",
	//	player->velocity.x, player->velocity.y ), vec2( 8.f, 24.f ) );
}

void scene_simple_top_down::update()
{
	scene::update();

	// show the raycast beam if the right stick is being pushed
	b_show_ray = g_engine->input.get_axis_state( input_id::gamepad_right_stick ).get_size_fast() > 0.f;

	//follow_cam( player->get_transform() );
}

void scene_simple_top_down::reset_collision_trace_results()
{
	hit_marker->get_component<ec_primitive_shape>()->shapes.clear();

	for( auto& iter : world->get_components<ec_simple_collision_body>() )
	{
		iter->rs_opt.color = make_color( color::dark_teal );
	}
}

bool scene_simple_top_down::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::key_1:
		{
			auto ec = player->get_component<ec_simple_collision_body>();
			ec->set_as_centered_box( radius * random::getf_range( 0.5f, 3.0f ), radius * random::getf_range( 0.5f, 3.0f ) );
		}
		break;

		case input_id::key_2:
		{
			auto ec = player->get_component<ec_simple_collision_body>();
			ec->set_as_circle( radius * random::getf_range( 0.5f, 2.0f ) );
		}
		break;

		case input_id::key_3:
		{
			auto ec = player->get_component<ec_simple_collision_body>();

			auto s = random::geti_range( 3, 8 );
			auto r = radius * random::getf_range( 0.5f, 3.0f );
			ec->set_as_polygon( util_geo::generate_convex_shape( s, r ) );
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

bool scene_simple_top_down::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			player->add_force( { evt->delta, 10.f } );

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

