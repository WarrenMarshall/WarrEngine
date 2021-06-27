
#include "app_header.h"

using namespace war;

// ----------------------------------------------------------------------------

scene_spatial_map::scene_spatial_map()
{
	flags.blocks_further_drawing = true;
	flags.requires_controller = true;
	flags.is_debug_physics_scene = true;
}

void scene_spatial_map::pushed()
{
	g_engine->window.set_mouse_mode( mouse_mode::os );

	spatial_map.init( rect( -viewport_hw, -viewport_hh, viewport_w, viewport_h ), 4, 2 );

	// an initial entity to get things rolling
	{
		auto e = add_entity<entity>();
		e->tag = H( "mario" );
		e->make_pickable();
		{
			auto ec = e->add_component<sprite_component>();
			ec->rs_opt.color = make_color( color::white, 1.f );
			ec->init( "anim_player_run" );
		}
		{
			auto ec = e->add_component<simple_collision_component>();
			ec->set_as_circle( 12.f );
			//ec->set_collision_flags( scene_simple_coll_mario, scene_simple_coll_geo );
			ec->rs_opt.color = make_color( color::orange );
		}
	}
}

void scene_spatial_map::draw()
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

void scene_spatial_map::draw_ui()
{
	scene::draw_ui();
	draw_title( "Quad Tree" );
}

bool scene_spatial_map::on_input_pressed( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::mouse_button_right:
		{
			// spawn a new entity at the location clicked

			vec2 click_pos = coord_system::window_to_viewport_pos( g_engine->input.mouse_window_pos );

			auto e = add_entity<entity>();
			e->tag = H( "mario" );
			e->make_pickable();
			e->set_pos( click_pos );
			{
				auto ec = e->add_component<sprite_component>();
				ec->rs_opt.color = make_color( color::white, 1.f );
				ec->init( "anim_player_run" );
			}
			{
				auto ec = e->add_component<simple_collision_component>();
				ec->set_as_circle( 12.f );
				ec->rs_opt.color = make_color( color::orange );
			}
		}
		break;

		case input_id::mouse_button_left:
		{
			vec2 click_pos = coord_system::window_to_viewport_pos( g_engine->input.mouse_window_pos );
			auto pick_id = render::sample_pick_id_at( click_pos );

			if( !evt->control_down )
			{
				deselect_all();
			}

			select_by_pick_id( pick_id );
		}
		break;
	}

	return false;
}

bool scene_spatial_map::on_input_motion( const input_event* evt )
{
	switch( evt->input_id )
	{
		case input_id::gamepad_left_stick:
		{
			return true;
		}
		break;

		case input_id::mouse:
		{
			if( !g_engine->input.get_button_state( input_id::mouse_button_left ) )
			{
				return false;
			}

			auto selected = get_selected();
			if( !selected.size() )
			{
				return false;
			}

			for( auto& e : selected )
			{
				e->add_delta_pos( fixed_time_step::per_second( evt->delta * 150.f ) );
			}

			return true;
		}
		break;
	}

	return false;
}
