
#include "app_header.h"

using namespace war;

scene_controller_callback::scene_controller_callback()
{
	progress_data_l2.draw_percentage_as_text = false;
	progress_data_r2.draw_percentage_as_text = false;
}

ui_control_data* scene_controller_callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "progress_l2" ):	return &progress_data_l2;
		case H( "progress_r2" ):	return &progress_data_r2;
	}

	return ui_callback::get_data( tag );
}


// ----------------------------------------------------------------------------

scene_controller::scene_controller()
{
	ui_callback = std::make_unique<scene_controller_callback>();
	draws_completely_solid = true;
}

void scene_controller::pushed()
{
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
	auto cb = (scene_controller_callback*)get_ui_callback();

	scene::draw_ui();

	draw_title( "Game Controller" );

	{
		scoped_render_state;

		render::state->color.a = g_engine->input.gamepad ? 1.f : 0.25f;
		render::draw_quad( g_engine->find_asset<texture_asset>( "gc_controller" ), vec2( 4.f, 0.f ) );

		if( g_engine->input.gamepad )
		{
			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_back ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_back" ), vec2( 52.f, 60.f ) );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_start ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_start" ), vec2( 102.f, 60.f ) );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_left_shoulder ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_l1" ), vec2( 188.f, 40.f ) );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_right_shoulder ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_r1" ), vec2( 238.f, 40.f ) );

			cb->progress_data_l2.set_float_value( g_engine->input.get_axis_state( input_id::gamepad_left_trigger ).y );
			g_ui
				->progress_control( H("progress_l2") )
				->set_pos( { 190.f, 116.f } )
				->set_size( { 46.f, 12.f } )
				->end();

			render::state->color.a = 0.25f + g_engine->input.get_axis_state( input_id::gamepad_left_trigger ).y;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_l2" ), vec2( 188.f, 73.f ) );

			cb->progress_data_r2.set_float_value( g_engine->input.get_axis_state( input_id::gamepad_right_trigger ).y );
			g_ui
				->progress_control( H( "progress_r2" ) )
				->set_pos( { 240.f, 116.f } )
				->set_size( { 46.f, 12.f } )
				->end();

			render::state->color.a = 0.25f + g_engine->input.get_axis_state( input_id::gamepad_right_trigger ).y;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_r2" ), vec2( 238.f, 73.f ) );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_left_thumb ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_stick_click" ), vec2( 35.f, 110.f ) );

			auto center = vec2( 60.f, 135.f );
			render::state->color.a = 1.f;
			auto lstick = g_engine->input.get_axis_state( input_id::gamepad_left_stick, use_controller_dead_zone ) * 24.f;
			auto lstick_pos = center + lstick;
			render::draw_filled_circle( lstick_pos, 4 );
			render::draw_line( center, lstick_pos );
			render::draw_filled_circle( center, 2 );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_right_thumb ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_stick_click" ), vec2( 123.f, 110.f ) );

			center = vec2( 147.f, 135.f );
			render::state->color.a = 1.f;
			auto rstick = g_engine->input.get_axis_state( input_id::gamepad_right_stick, use_controller_dead_zone ) * 24.f;
			auto rstick_pos = center + rstick;
			render::draw_filled_circle( rstick_pos, 4 );
			render::draw_line( center, rstick_pos );
			render::draw_filled_circle( center, 2 );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_up ) ? 1.f : 0.5f;
			render::draw_sprite( g_engine->find_asset<texture_asset>( "gc_dpad_up" ), vec2( 102.f, 160.f ) );

			render::state->angle = 90;
			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_right ) ? 1.f : 0.5f;
			render::draw_sprite( g_engine->find_asset<texture_asset>( "gc_dpad_up" ), vec2( 130.f, 182.f ) );

			render::state->angle = 180;
			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_down ) ? 1.f : 0.5f;
			render::draw_sprite( g_engine->find_asset<texture_asset>( "gc_dpad_up" ), vec2( 102.f, 204.f ) );

			render::state->angle = 270;
			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_left ) ? 1.f : 0.5f;
			render::draw_sprite( g_engine->find_asset<texture_asset>( "gc_dpad_up" ), vec2( 74.f, 182.f ) );

			render::state->angle = 0;
			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_y ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_y" ), vec2( 229.f - 16.f, 139.f ) );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_b ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_b" ), vec2( 261.f - 16.f, 158.f ) );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_a ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_a" ), vec2( 229.f - 16.f, 179.f ) );

			render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_x ) ? 1.f : 0.5f;
			render::draw_quad( g_engine->find_asset<texture_asset>( "gc_button_x" ), vec2( 197.f - 16.f, 158.f ) );

			render::state->color = make_color( color::black, 0.5f );
			render::draw_filled_rect( rect( 0.f, ui_h - 14.f, ui_w, 14.f ) );

			render::state->align = align::centered;
			render::state->color = make_color( 4, 1.f );
			render::draw_string( std::format( "Use controller 'dead zone': {}", use_controller_dead_zone ), vec2( ui_hw, ui_h - 8.f ) );

			return;
		}

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
