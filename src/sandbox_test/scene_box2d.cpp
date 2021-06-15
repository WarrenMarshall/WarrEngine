
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_box2d_all = ++collision_bits;
static const unsigned scene_box2d_world = ++collision_bits;
static const unsigned scene_box2d_ball = ++collision_bits;

// ----------------------------------------------------------------------------

scene_box2d::scene_box2d()
{
	flags.blocks_further_drawing = true;
	flags.is_debug_physics_scene = true;
}

void scene_box2d::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// kinematic circle
	{
		auto e = add_entity<entity>();
		e->tag = H( "main_ball" );
		{
			{
				auto ec = e->add_component<kinematic_physics_body_component>();
				ec->add_fixture_circle( hash_none, { 0.f, 0.f }, 32.f );
				ec->set_collision_flags( scene_box2d_world, scene_box2d_ball );
			}
			{
				auto ec = e->add_component<primitive_shape_component>();
				ec->add_shape( primitive_shape::filled_circle, 32.f );
				ec->rs_opt.color = make_color( pal::brighter );
				ec->rs_opt.glow = 1.f;
			}
		}
	}

	// static world geometry
	{
		auto e = add_entity<entity>();
		e->tag = H( "world" );
		{
			rect rc_floor = { -viewport_hw, 0.f, viewport_w, 4.f };
			rect rc_left_wall = { -viewport_hw, -viewport_hh * 6.f, 8.f, viewport_hh * 6.f };
			rect rc_right_wall = { viewport_hw - 8.f, -viewport_hh * 6.f, 8.f, viewport_hh * 6.f };
			{
				auto ec = e->add_component<static_physics_body_component>();
				ec->add_fixture_box( hash_none, rc_floor );
				ec->add_fixture_box( hash_none, rc_left_wall );
				ec->add_fixture_box( hash_none, rc_right_wall );
				e->get_component<physics_component>()->set_collision_flags( scene_box2d_world, scene_box2d_ball );
			}
			{
				auto ec = e->add_component<primitive_shape_component>();
				ec->add_shape( primitive_shape::filled_rect, rc_floor );
				ec->add_shape( primitive_shape::filled_rect, rc_left_wall );
				ec->add_shape( primitive_shape::filled_rect, rc_right_wall );
				ec->rs_opt.color = make_color( pal::brighter );
			}
		}
	}
}

void scene_box2d::draw()
{
	{
		scoped_render_state;

		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ), rect( -viewport_hw, -viewport_h, viewport_w, viewport_h ) );
	}

	scene::draw();
	render::draw_world_axis();
}

void scene_box2d::draw_ui()
{
	scene::draw_ui();
	draw_title( "Box2D Physics" );
}

// spawns a randomly sized emoji ball at "world_pos"

void scene_box2d::spawn_ball_at( vec2 world_pos )
{
	auto e = add_entity<e_emoji_ball>();
	e->transform_set_pos( world_pos );
	e->rs_opt.color = color( random::getf(), random::getf(), random::getf() );
	e->make_pickable();
	{
		float random_radius = random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<dynamic_physics_body_component>();
			ec->is_primary_body = true;
			ec->add_fixture_circle( hash_none, vec2::zero, random_radius );
			ec->set_collision_flags( scene_box2d_ball, scene_box2d_ball | scene_box2d_world );
			e->get_component<physics_component>()->set_restitution( random::getf_range( 0.f, 1.f ) );
		}
		{
			auto ec = e->add_component<sprite_component>();
			ec->init( std::format( "emoji_{}", random::geti_range( 1, 5 ) ) );
			ec->get_transform()->set_scale( random_radius / 8.f );
			ec->rs_opt.glow = random::getb() ? random::getf_range( 0.f, 2.0f ) : 0.f;
		}
	}
}

// spawns a randomly sized and colored box at "world_pos"

void scene_box2d::spawn_box_at( vec2 world_pos )
{
	float base_size = 8.f;
	float w = random::getf_range( base_size, base_size * 10.f );
	float h = ( base_size * 15.f ) - w;
	rect rc_box = { 0.f, 0.f, w, h };

	auto e = add_entity<entity>();
	e->transform_set_pos( world_pos );
	e->make_pickable();
	{
		float random_radius = random::getf_range( 16.f, 32.f );

		{
			auto ec = e->add_component<dynamic_physics_body_component>();
			ec->is_primary_body = true;
			ec->add_fixture_box( hash_none, rc_box );
			ec->set_collision_flags( scene_box2d_ball, scene_box2d_ball | scene_box2d_world );
			e->get_component<physics_component>()->set_restitution( random::getf_range( 0.f, 1.f ) );
		}
		rc_box.grow( 1.0f );
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->add_shape( primitive_shape::filled_rect, rc_box );
			ec->rs_opt.color = make_color( pal::darker );
		}
		rc_box.shrink( 2.0f );
		{
			auto ec = e->add_component<primitive_shape_component>();
			ec->add_shape( primitive_shape::filled_rect, rc_box );

			glm::vec3 clr = { random::getf(), random::getf(), random::getf() };
			auto sz = glm::length( clr );
			ec->rs_opt.color = color( clr.r / sz, clr.g / sz, clr.b / sz );
			ec->rs_opt.glow = random::getb() ? random::getf_range( 0.f, 2.f ) : 0.f;
		}
	}
}

bool scene_box2d::on_input_pressed( const input_event* evt )
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

bool scene_box2d::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse:
		{
			if( g_engine->input.get_button_state( input_id::mouse_button_left ) == button_state::held )
			{
				if( !evt->shift_down and !evt->control_down )
				{
					auto world_pos = coord_system::window_to_world_pos( evt->mouse_pos );

					auto e = find_entity( H( "main_ball" ) );
					e->transform_set_pos( world_pos );

					return true;
				}
			}
		}
		break;
	}

	return false;
}
