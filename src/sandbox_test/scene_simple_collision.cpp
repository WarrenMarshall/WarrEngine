
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits( 1 );

static const unsigned scene_simple_all = collision_bits.next();
static const unsigned scene_simple_world = collision_bits.next();
static const unsigned scene_simple_ball = collision_bits.next();

// ----------------------------------------------------------------------------

scene_simple_collision::scene_simple_collision()
{
	draws_completely_solid = true;
	is_debug_physics_scene = true;
}

void scene_simple_collision::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	{
		auto e = add_entity<entity>();
		e->tag = H( "player_mario" );
		e->make_pickable();
		e->transform_set_pos( { 0.f, 0.f } );
		e->transform_set_scale( 3.f );
		{
			auto ec = e->add_component<sprite_component>();
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<simple_collision_component>();
			ec->init( 32.f, 16.f );
		}
	}

	// entity 1
/*
	{
		auto e = add_entity<entity>();
		e->tag = H( "main_ball" );
		{
			{
				auto ec = e->add_component<kinematic_physics_body_component>();
				ec->add_fixture_circle( hash_none, { 0.f, 0.f }, 32.f );
				ec->set_collision_flags( scene_simple_world, scene_simple_ball );
			}
			{
				auto ec = e->add_component<primitive_shape_component>();
				ec->add_shape( primitive_shape::filled_circle, 32.f );
				ec->rs_opt.color = make_color( pal::brighter );
				ec->rs_opt.glow = 1.f;
			}
		}
	}
*/

	// static world geometry
/*
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
				e->get_component<physics_component>()->set_collision_flags( scene_simple_world, scene_simple_ball );
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
*/
}

void scene_simple_collision::draw()
{
	{
		scoped_render_state;

		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ), rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	//render::draw_world_axis();
}

void scene_simple_collision::draw_ui()
{
	scene::draw_ui();
	draw_title( "Simple Collisions" );
}

bool scene_simple_collision::on_input_pressed( const input_event* evt )
{
	return false;
}

bool scene_simple_collision::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse:
		{
		}
		break;
	}

	return false;
}
