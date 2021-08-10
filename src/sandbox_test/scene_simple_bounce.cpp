
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_space_coll_player = collision_bits.get();
static const unsigned scene_simple_space_coll_geo = collision_bits.next();

// ----------------------------------------------------------------------------

scene_simple_bounce::scene_simple_bounce()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

entity* scene_simple_bounce::spawn_player()
{
	constexpr auto radius = 12.f;
	auto e = add_entity<entity>();
	e->set_scale( random::getf_range( 1.0f, 2.0f ) );
	e->simple.friction = 0.0f;
	e->simple.max_velocity = 5.0f;
	e->simple.is_bouncy = true;

	{
		auto ec = e->add_component<ec_simple_collision_body>();

		switch( random::geti_range( 0, 2 ) )
		{
			case 0:
			{
				ec->set_as_centered_box( radius * random::getf_range( 0.5f, 3.0f ), radius * random::getf_range( 0.5f, 3.0f ) );
			}
			break;

			case 1:
			{
				ec->set_as_circle( radius * random::getf_range( 0.5f, 2.0f ) );
			}
			break;

			case 2:
			{
				auto s = random::geti_range( 3, 8 );
				auto r = radius * random::getf_range( 0.5f, 3.0f );
				ec->set_as_polygon( util_geo::generate_convex_shape( s, r ) );
			}
			break;
		}

		ec->set_collision_flags( scene_simple_space_coll_player, scene_simple_space_coll_geo | scene_simple_space_coll_player );
	}
	{
		auto ec = e->add_component<ec_primitive_shape>();
		ec->rs_opt.color = make_color( color::white, 1.0f );
		ec->add_shape( primitive_shape::point );
	}

	if( first_time )
	{
		player = e;
	}

	e->apply_impulse( { random::get_random_unit_vector(), 2.f } );

	first_time = false;

	return e;
}

void scene_simple_bounce::pushed()
{
	scene::pushed();

	g_engine->renderer.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// WORLD GEO

	{
		int num_colliders = 5;

		auto e = add_entity<entity>();
		e->tag = H( "world_geo" );
		e->simple.type = sc_type::stationary;

		for( int i = 0 ; i < num_colliders ; ++i )
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
		}

		for( int i = 0 ; i < num_colliders ; ++i )
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
		}

		// 4 walls
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_space_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_space_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_space_coll_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_space_coll_geo, 0 );
		}

		world_geo = e;
	}
}

void scene_simple_bounce::draw()
{
	{
		scoped_render_state;
		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ),
			rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	//render::draw_world_axis();
}

void scene_simple_bounce::draw_ui()
{
	scene::draw_ui();
	//draw_title( "Space Drifter" );

	//if( player )
	//{
	//	render::draw_string( std::format( "Velocity : {:.1f}, {:.1f}",
	//		player->velocity.x, player->velocity.y ), vec2( 8.f, 24.f ) );
	//}
}

void scene_simple_bounce::post_update()
{
	scene::post_update();

	for( auto& e : entities )
	{
		auto pos = e->get_pos();

		if( pos.x < -viewport_hw )		pos.x = viewport_hw;
		if( pos.x > viewport_hw )		pos.x = -viewport_hw;
		if( pos.y < -viewport_hh )		pos.y = viewport_hh;
		if( pos.y > viewport_hh )		pos.y = -viewport_hh;

		e->set_pos( pos );
	}
}

bool scene_simple_bounce::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_button_y:
		case input_id::key_space:
		{
			spawn_player();
		}
		break;
	}

	return false;
}

bool scene_simple_bounce::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			player->apply_force( { evt->delta, 1.0f } );

			return true;
		}
		break;
	}

	return false;
}
