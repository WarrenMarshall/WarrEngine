
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

static bit_flag_generator collision_bits = 1;

static const unsigned scene_simple_coll_mario = ++collision_bits;
static const unsigned scene_simple_coll_skull = ++collision_bits;

// ----------------------------------------------------------------------------

scene_simple_collision::scene_simple_collision()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = true;
	flags.is_debug_physics_scene = true;
}

void scene_simple_collision::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	// MARIO
	{
		auto e = add_entity<entity>();
		e->tag = H( "mario" );
		e->debug_name = "MARIO";
		e->transform_set_pos( { -80.f, 0.f } );
		//e->transform_set_scale( 3.f );
		{
			auto ec = e->add_component<sprite_component>();
			ec->rs_opt.color = make_color( color::white, 0.25f );
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<simple_collision_component>();
			//ec->set_as_centered_box( 24.f, 24.f );
			ec->set_as_circle( 24.f );
			ec->set_collision_flags( scene_simple_coll_mario, scene_simple_coll_skull );
		}

		mario = e;
	}

	// SKULL
	{
		auto e = add_entity<entity>();
		e->tag = H( "skull" );
		e->debug_name = "SKULL";
		e->transform_set_pos( { 80.f, 0.f } );
		//e->transform_set_scale( 2.f );
		{
			auto ec = e->add_component<sprite_component>();
			ec->rs_opt.color = make_color( color::white, 0.25f );
			ec->init( "tex_skull" );
		}
		{
			auto ec = e->add_component<simple_collision_component>();
			//ec->set_as_centered_box( 24.f, 24.f );
			ec->set_as_circle( 24.f );
			ec->set_collision_flags( scene_simple_coll_skull, 0 );
		}

		skull = e;
	}
}

void scene_simple_collision::draw()
{
	{
		scoped_render_state;
		render::state->color = make_color( pal::darker );
		render::draw_tiled( g_engine->find_asset<texture_asset>( "engine_tile_background_stripe" ), rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ) );
	}

	scene::draw();
	render::draw_world_axis();

	if( b_last_collision )
	{
		scoped_render_state;

		render::state->z += 5.f;
		render::state->color = make_color( color::green );
		auto start = last_collision.entity_a->get_transform()->pos;// last_collision.closest_point;
		auto end = start + ( last_collision.normal * last_collision.depth );
		render::draw_line( start, end );

		render::state->color = make_color( color::red );
		render::state->z += 5.f;
		render::draw_point( last_collision.closest_point );
	}
}

void scene_simple_collision::draw_ui()
{
	scene::draw_ui();
	draw_title( "Simple Collisions" );

	if( b_last_collision )
	{
		render::state->color = color::light_green;
		auto ypos = 32.f;
		render::draw_string( std::format( "count  : {}", last_collision.manifold.count ), vec2( 4.f, ypos ) );
		ypos += 10.f;
		render::draw_string( std::format( "closest: {}, {}", last_collision.closest_point.x, last_collision.closest_point.y ), vec2( 4.f, ypos ) );
		ypos += 10.f;
		render::draw_string( std::format( "depth  : {}", last_collision.manifold.depths[0] ), vec2( 4.f, ypos ) );
		ypos += 10.f;
		render::draw_string( std::format( "normal : {},{}", last_collision.manifold.n.x, last_collision.manifold.n.y ), vec2( 4.f, ypos ) );
	}
}

bool scene_simple_collision::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::key_space )
	{
		if( b_last_collision )
		{
			last_collision.entity_a->transform_delta_pos( last_collision.normal * last_collision.depth );
		}
	}

	return false;
}

bool scene_simple_collision::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			mario->transform_delta_pos( evt->delta * 2.f );
		}
		break;

		case input_id::gamepad_right_stick:
		{
			skull->transform_delta_pos( evt->delta * 2.f );
		}
		break;
	}

	return false;
}
