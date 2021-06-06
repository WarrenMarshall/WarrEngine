
#include "app_header.h"

using namespace war;

scene_controller::scene_controller()
{
	draws_completely_solid = true;
}

void scene_controller::pushed()
{
	progress_data_l2.set_float_value( 0.f );
	progress_data_r2.set_float_value( 0.f );

	viewport_pivot = vec2::zero;
	fader = tween( 0.25f, 1.f, 1000, tween_type::pingpong, tween_via::sinusoidal );
	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_controller::draw()
{
	draw_tiled_background();
	scene::draw();
	render::draw_world_axis();
}

void scene_controller::draw_ui()
{
	scene::draw_ui();
	draw_title( "Game Controller" );

	{
		scoped_render_state;

		render::state->color.a = g_engine->input.gamepad ? 1.f : 0.25f;
		render::draw_quad( g_engine->find_asset<texture_asset>( "gc_controller" ), vec2( 4.f, 0.f ) );

/*
		if( g_engine->input.gamepad )
		{
			w_render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_back ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_back" ), w_vec2( 52.f, 60.f ) );

			w_render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_start ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_start" ), w_vec2( 102.f, 60.f ) );

			w_render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_left_shoulder ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_l1" ), w_vec2( 188.f, 40.f ) );

			w_render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_right_shoulder ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_r1" ), w_vec2( 238.f, 40.f ) );

			progress_data_l2 = g_engine->input.get_axis_state( input_id::gamepad_left_trigger ).y;
			ui
				->progress_control( H("progress_l2") )
				->set_pos( { 190.f, 116.f } )
				->set_size( { 46.f, 12.f } )
				->finalize( &progress_data_l2 );

			w_render::state->color.a = 0.25f + engine->input->get_axis_state( input_id::gamepad_left_trigger ).y;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_l2" ), w_vec2( 188.f, 73.f ) );

			progress_data_r2 = engine->input->get_axis_state( input_id::gamepad_right_trigger ).y;
			ui
				->progress_control( H( "progress_r2" ) )
				->set_pos( { 240.f, 116.f } )
				->set_size( { 46.f, 12.f } )
				->finalize( &progress_data_r2 );

			w_render::state->color.a = 0.25f + engine->input->get_axis_state( input_id::gamepad_right_trigger ).y;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_r2" ), w_vec2( 238.f, 73.f ) );

			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_left_thumb ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_stick_click" ), w_vec2( 35.f, 110.f ) );

			auto center = w_vec2( 60.f, 135.f );
			w_render::state->color.a = 1.f;
			auto lstick = engine->input->get_axis_state( input_id::gamepad_left_stick, use_controller_dead_zone ) * 24.f;
			auto lstick_pos = center + lstick;
			w_render::draw_filled_circle( lstick_pos, 4 );
			w_render::draw_line( center, lstick_pos );
			w_render::draw_filled_circle( center, 2 );

			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_right_thumb ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_stick_click" ), w_vec2( 123.f, 110.f ) );

			center = w_vec2( 147.f, 135.f );
			w_render::state->color.a = 1.f;
			auto rstick = engine->input->get_axis_state( input_id::gamepad_right_stick, use_controller_dead_zone ) * 24.f;
			auto rstick_pos = center + rstick;
			w_render::draw_filled_circle( rstick_pos, 4 );
			w_render::draw_line( center, rstick_pos );
			w_render::draw_filled_circle( center, 2 );

			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_dpad_up ) ? 1.f : 0.5f;
			w_render::draw_sprite( g_engine->find_asset_in_cache<texture_asset>( "gc_dpad_up" ), w_vec2( 102.f, 160.f ) );

			w_render::state->angle = 90;
			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_dpad_right ) ? 1.f : 0.5f;
			w_render::draw_sprite( g_engine->find_asset_in_cache<texture_asset>( "gc_dpad_up" ), w_vec2( 130.f, 182.f ) );

			w_render::state->angle = 180;
			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_dpad_down ) ? 1.f : 0.5f;
			w_render::draw_sprite( g_engine->find_asset_in_cache<texture_asset>( "gc_dpad_up" ), w_vec2( 102.f, 204.f ) );

			w_render::state->angle = 270;
			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_dpad_left ) ? 1.f : 0.5f;
			w_render::draw_sprite( g_engine->find_asset_in_cache<texture_asset>( "gc_dpad_up" ), w_vec2( 74.f, 182.f ) );

			w_render::state->angle = 0;
			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_y ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_y" ), w_vec2( 229.f - 16.f, 139.f ) );

			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_b ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_b" ), w_vec2( 261.f - 16.f, 158.f ) );

			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_a ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_a" ), w_vec2( 229.f - 16.f, 179.f ) );

			w_render::state->color.a = engine->input->is_button_down( input_id::gamepad_button_x ) ? 1.f : 0.5f;
			w_render::draw_quad( g_engine->find_asset_in_cache<texture_asset>( "gc_button_x" ), w_vec2( 197.f - 16.f, 158.f ) );

			w_render::state->color = make_color( clr::black, 0.5f );
			w_render::draw_filled_rect( w_rect( 0.f, ui_h - 14.f, ui_w, 14.f ) );

			w_render::state->align = align::centered;
			w_render::state->color = make_color( 4, 1.f );
			w_render::draw_string( std::format( "Use controller 'dead zone': {}", use_controller_dead_zone ), w_pos( ui_hw, ui_h - 8.f ) );

			return;
		}
*/

		render::state->color = make_color( color::black, 0.5f );
		render::draw_filled_rect( rect( 0.f, ui_hh - 7.f, ui_w, 14.f ) );

		render::state->align = align::centered;
		render::state->color = make_color( 3, *fader );
		render::draw_string( "no game controller detected - plug one in!", vec2( ui_hw, ui_hh ) );
	}
}

bool scene_controller::on_input_pressed( const input_event* evt )
{
	if( evt->input_id == input_id::key_d )
	{
		toggle_bool( use_controller_dead_zone );
		return true;
	}

	return false;
}
