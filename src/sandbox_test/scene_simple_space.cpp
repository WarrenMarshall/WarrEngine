
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

entity* scene_simple_space::spawn_player()
{
	constexpr auto radius = 12.f;
	auto e = add_entity<entity>();
	e->set_scale( random::getf_range( 1.0f, 2.0f ) );
	e->simple.damping = 0.05f;
	e->simple.max_velocity = 5.0f;

	{
		auto ec = e->add_component<ec_sprite>();
		ec->rs_opt.color = make_color( color::white, 1.f );
		ec->init( "anim_player_run" );
	}
	{
		auto ec = e->add_component<ec_simple_collision_body>();

	#if 1
		if( random::getb() )
			ec->set_as_circle( radius );
		else
			ec->set_as_centered_box( radius * 2.f, radius * 2.f );
	#else
		ec->set_as_circle( radius );
	#endif

		ec->set_collision_flags( scene_simple_space_coll_mario, scene_simple_space_coll_geo | scene_simple_space_coll_mario );
	}
	{
		auto ec = e->add_component<ec_primitive_shape>();
		ec->rs_opt.color = make_color( color::green, 0.25f );
		ec->add_shape( primitive_shape::filled_circle, radius );
	}

	if( first_time )
	{
		mario = e;
	}

	e->apply_impulse( { random::get_random_unit_vector(), 500.f } );

	first_time = false;

	return e;
}

void scene_simple_space::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( mouse_mode::os );

	// MARIO
	//mario = spawn_player();

	// WORLD GEO

	{
		int num_colliders = 10;

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

		e = add_entity<entity>();
		e->tag = H( "world_geo" );
		e->simple.type = sc_type::stationary;

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
	draw_title( "Space Drifter" );

	if( mario )
	{
		render::draw_string( std::format( "Velocity : {:.1f}, {:.1f}",
			mario->velocity.x, mario->velocity.y ), vec2( 8.f, 24.f ) );
	}
}

void scene_simple_space::post_update()
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

bool scene_simple_space::on_input_pressed( const input_event* evt )
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

bool scene_simple_space::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			mario->apply_force( { evt->delta, 1.0f } );

			return true;
		}
		break;
	}

	return false;
}
