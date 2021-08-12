
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_coll_ball = collision_bits.get();
static const unsigned scene_simple_coll_world = collision_bits.next();
static const unsigned scene_simple_coll_dynamic_object = collision_bits.get();

scene_simple_collision::scene_simple_collision()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = false;
	flags.is_debug_physics_scene = true;
}

void scene_simple_collision::pushed()
{
	scene::pushed();

	g_engine->renderer.debug.draw_debug_info = true;
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// KINEMATIC CIRCLE
	{
		auto e = add_entity<entity>();
		e->tag = H( "main_ball" );
		e->simple.type = sc_type::kinematic;
		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->add_shape( primitive_shape::point );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->set_as_circle( 32.f );
			ec->set_collision_flags( scene_simple_coll_ball, scene_simple_coll_dynamic_object );
		}
	}

	// WORLD GEO

	{
		auto e = add_entity<entity>();
		e->simple.type = sc_type::stationary;
		e->set_debug_name( "world" );

		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw, viewport_hh - 8.f } );
			ec->set_as_box( viewport_w, 16.f );
			ec->set_collision_flags( scene_simple_coll_world, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { -viewport_hw - 8.f, -viewport_h } );
			ec->set_as_box( 16.f, viewport_h * 2.f );
			ec->set_collision_flags( scene_simple_coll_world, 0 );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->get_transform()->set_pos( { viewport_hw - 8.f, -viewport_h } );
			ec->set_as_box( 16.f, viewport_h * 2.f );
			ec->set_collision_flags( scene_simple_coll_world, 0 );
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
	//render::draw_world_axis();
}

void scene_simple_collision::draw_ui()
{
	scene::draw_ui();
}

void scene_simple_collision::update()
{
	scene::update();
}

void scene_simple_collision::spawn_ball_at( vec2 world_pos )
{
	auto e = add_entity<entity>();
	e->set_debug_name( "ball" );
	e->set_pos( world_pos );
	e->rs_opt.color = color( random::getf(), random::getf(), random::getf() );
	e->make_pickable();
	e->simple.is_bouncy = false;
	e->simple.friction = 0.01f;
	e->simple.is_affected_by_gravity = true;
	e->simple.max_velocity_y = { -5.f, simple_collision_gravity_max };
	e->simple.is_bouncy = true;
	{
		float random_radius = random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->add_shape( primitive_shape::point );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->set_as_circle( random_radius );
			ec->set_collision_flags( scene_simple_coll_dynamic_object, scene_simple_coll_world | scene_simple_coll_ball );
		}
	}
}

void scene_simple_collision::spawn_box_at( vec2 world_pos )
{
	float base_size = 8.f;
	float w = random::getf_range( base_size, base_size * 10.f );
	float h = ( base_size * 15.f ) - w;
	rect rc_box = { 0.f, 0.f, w, h };

	auto e = add_entity<entity>();
	e->set_pos( world_pos );
	e->make_pickable();
	e->simple.friction = 0.5f;
	e->simple.is_affected_by_gravity = true;
	e->simple.max_velocity_y = { -5.f, simple_collision_gravity_max };
	{
		float random_radius = random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<ec_primitive_shape>();
			ec->add_shape( primitive_shape::point );
		}
		{
			auto ec = e->add_component<ec_simple_collision_body>();
			ec->set_as_box( rc_box.w, rc_box.h );
			ec->set_collision_flags( scene_simple_coll_dynamic_object, scene_simple_coll_world | scene_simple_coll_ball );
		}
	}
}

bool scene_simple_collision::on_input_pressed( const input_event* evt )
{
	// spawn ball at random location
	if( evt->input_id == input_id::key_r )
	{
		auto num_new_balls = 1;

		if( evt->shift_down )
		{
			num_new_balls = 20;
		}

		range<float> spawn_x( -viewport_hw + 8, +viewport_hw - 8 );
		range<float> spawn_y( -viewport_hh, -8.f );

		for( auto x = 0 ; x < num_new_balls ; ++x )
		{
			spawn_ball_at( { spawn_x.get_random_value(), spawn_y.get_random_value() } );
		}
	}

	// delete entities with right click
	if( evt->input_id == input_id::mouse_button_right )
	{
		auto pick_id = render::sample_pick_id_at( coord_system::window_to_viewport_pos( evt->mouse_pos ) );
		auto e = find_entity_by_pick_id( pick_id );

		if( e )
		{
			e->set_life_cycle( life_cycle::dying );
			g_engine->find_asset<sound_asset>( "sfx_entity_delete" )->play();
		}
	}

	// shift_lclick to spawn new ball at mouse position
	if( evt->input_id == input_id::mouse_button_left and evt->shift_down )
	{
		auto world_click_location = coord_system::window_to_world_pos( evt->mouse_pos );
		spawn_ball_at( world_click_location );
		return true;
	}

	// control_lclick to spawn new box at mouse position
	if( evt->input_id == input_id::mouse_button_left and evt->control_down )
	{
		auto world_click_location = coord_system::window_to_world_pos( evt->mouse_pos );
		spawn_box_at( world_click_location );
		return true;
	}

	return false;
}

bool scene_simple_collision::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse:
		{
			if( g_engine->input.is_button_held( input_id::mouse_button_left ) )
			{
				if( !evt->shift_down and !evt->control_down )
				{
					auto world_pos = coord_system::window_to_world_pos( evt->mouse_pos );

					auto e = find_entity( H( "main_ball" ) );
					e->set_pos( world_pos );

					return true;
				}
			}
		}
		break;
	}

	return false;
}
