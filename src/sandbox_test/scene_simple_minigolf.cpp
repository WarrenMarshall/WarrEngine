
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_minigolf_player = collision_bits.get();
static const unsigned scene_simple_minigolf_geo = collision_bits.next();

// ----------------------------------------------------------------------------

scene_simple_minigolf::scene_simple_minigolf()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void scene_simple_minigolf::pushed()
{
	scene::pushed();

	g_engine->window.set_mouse_mode( mouse_mode::os );

	// PLAYER
	{
		auto e = add_entity<entity>();
		e->tag = H( "player" );
		e->set_pos( { -80.f, 0.f } );
		e->set_scale( 2.0f );
		e->simple.friction = 0.1f;
		{
			auto ec = e->add_component<ec_sprite>();
			ec->rs_opt.color = make_color( color::white, 1.f );
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->set_as_circle( 8.f );
			ec->set_collision_flags( scene_simple_minigolf_player, scene_simple_minigolf_geo );
		}

		player = e;
	}

	// WORLD GEO

	{
		constexpr auto num_circles = 5;
		constexpr auto num_boxes = 5;

		auto e = add_entity<entity>();
		e->tag = H( "world_geo" );
		e->simple.type = sc_type::stationary;

		for( int i = 0 ; i < num_boxes ; ++i )
		{
			auto x = random::getf_range( -viewport_hw, viewport_hw );
			auto y = random::getf_range( -viewport_hw, viewport_hw );
			auto w = random::getf_range( 16.f, 80.f );
			auto h = random::getf_range( 16.f, 80.f );

			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_as_centered_box( w, h );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_minigolf_geo, 0 );
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

		for( int i = 0 ; i < num_circles ; ++i )
		{
			auto x = random::getf_range( -viewport_hw, viewport_hw );
			auto y = random::getf_range( -viewport_hw, viewport_hw );
			auto r = random::getf_range( 8.f, 40.f );

			{
				auto ec = e->add_component<ec_simple_collision_body>();
				ec->set_as_circle( r );
				ec->get_transform()->set_pos( { x, y } );
				ec->set_collision_flags( scene_simple_minigolf_geo, 0 );
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
			ec->set_collision_flags( scene_simple_minigolf_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw, -viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_minigolf_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_minigolf_geo, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_hh } );
			ec->set_as_box( 16.f, viewport_h );
			ec->set_collision_flags( scene_simple_minigolf_geo, 0 );
		}

		world_geo = e;
	}
}

void scene_simple_minigolf::draw()
{
	{
		scoped_render_state;
		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ),
			rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	render::draw_world_axis();

	if( b_is_aiming )
	{
		auto start = player->get_pos();

		auto aim_dir_normalized = vec2::normalize( aim_dir );

		render::state->color = make_color( color::grey );
		render::draw_line( start, start + ( aim_dir_normalized * 64.f ) );

		render::state->color = make_color( color::orange );
		render::draw_line( start, start + ( aim_dir_normalized * (64.f * aim_force ) ) );
	}
}

void scene_simple_minigolf::draw_ui()
{
	scene::draw_ui();
	draw_title( "Mini Golf" );
}

void scene_simple_minigolf::update()
{
	scene::update();

	b_is_aiming = false;

	if( player->velocity.get_size() < 1.f )
	{
		// show the aim beam if the left stick is being pushed
		b_is_aiming = g_engine->input.get_axis_state( input_id::gamepad_left_stick ).get_size_fast() > 0.f;
	}
}

bool scene_simple_minigolf::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_button_a:
		{
			if( b_is_aiming )
			{
				player->apply_impulse( { -aim_dir, aim_force * 500.f } );
			}
			return true;
		}
		break;
	}

	return false;
}

bool scene_simple_minigolf::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			aim_dir = evt->delta;
			aim_force = aim_dir.get_size();
			return true;
		}
		break;
	}

	return false;
}
