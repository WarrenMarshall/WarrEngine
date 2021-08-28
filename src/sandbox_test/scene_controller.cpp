
#include "app_header.h"

using namespace war;

scene_controller_callback::scene_controller_callback()
{
	progress_data_l2.draw_percentage_as_text = false;
	progress_data_r2.draw_percentage_as_text = false;
}

UI_Control_Data* scene_controller_callback::get_data( hash tag )
{
	switch( tag )
	{
		case H( "progress_l2" ):	return &progress_data_l2;
		case H( "progress_r2" ):	return &progress_data_r2;
	}

	return UI_Callback::get_data( tag );
}


// ----------------------------------------------------------------------------

scene_controller::scene_controller()
{
	ui_callback = std::make_unique<scene_controller_callback>();
	flags.blocks_further_drawing = true;
}

void scene_controller::pushed()
{
	scene::pushed();

	viewport_pivot = Vec2::zero;
	fader = Tween( 0.25f, 1.f, 1000, tween_type::pingpong, tween_via::sinusoidal );
	g_engine->window.set_mouse_mode( mouse_mode::os );
}

void scene_controller::draw()
{
	draw_tiled_background();
	scene::draw();
	Render::draw_world_axis();
}

void scene_controller::draw_ui()
{
	auto cb = (scene_controller_callback*)get_ui_callback();

	scene::draw_ui();

	draw_title( "Game Controller" );

	{
		scoped_render_state;

		Render::state->color.a = g_engine->input.gamepad ? 1.f : 0.25f;
		Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_controller" ), Vec2( 4.f, 0.f ) );

		if( g_engine->input.gamepad )
		{
			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_back ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_back" ), Vec2( 52.f, 60.f ) );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_start ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_start" ), Vec2( 102.f, 60.f ) );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_left_shoulder ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_l1" ), Vec2( 188.f, 40.f ) );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_right_shoulder ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_r1" ), Vec2( 238.f, 40.f ) );

			cb->progress_data_l2.set_float_value( g_engine->input.get_axis_state( input_id::gamepad_left_trigger ).y );
			g_ui
				->progress_control( H("progress_l2") )
				->set_pos( { 190.f, 116.f } )
				->set_size( { 46.f, 12.f } )
				->done();

			Render::state->color.a = 0.25f + g_engine->input.get_axis_state( input_id::gamepad_left_trigger ).y;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_l2" ), Vec2( 188.f, 73.f ) );

			cb->progress_data_r2.set_float_value( g_engine->input.get_axis_state( input_id::gamepad_right_trigger ).y );
			g_ui
				->progress_control( H( "progress_r2" ) )
				->set_pos( { 240.f, 116.f } )
				->set_size( { 46.f, 12.f } )
				->done();

			Render::state->color.a = 0.25f + g_engine->input.get_axis_state( input_id::gamepad_right_trigger ).y;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_r2" ), Vec2( 238.f, 73.f ) );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_left_thumb ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_stick_click" ), Vec2( 35.f, 110.f ) );

			auto center = Vec2( 60.f, 135.f );
			Render::state->color.a = 1.f;
			auto lstick = g_engine->input.get_axis_state( input_id::gamepad_left_stick, use_controller_dead_zone ) * 24.f;
			auto lstick_pos = center + lstick;
			Render::draw_filled_circle( lstick_pos, 4 );
			Render::draw_line( center, lstick_pos );
			Render::draw_filled_circle( center, 2 );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_right_thumb ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_stick_click" ), Vec2( 123.f, 110.f ) );

			center = Vec2( 147.f, 135.f );
			Render::state->color.a = 1.f;
			auto rstick = g_engine->input.get_axis_state( input_id::gamepad_right_stick, use_controller_dead_zone ) * 24.f;
			auto rstick_pos = center + rstick;
			Render::draw_filled_circle( rstick_pos, 4 );
			Render::draw_line( center, rstick_pos );
			Render::draw_filled_circle( center, 2 );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_up ) ? 1.f : 0.5f;
			Render::draw_sprite( g_engine->find_asset<Texture_Asset>( "gc_dpad_up" ), Vec2( 102.f, 160.f ) );

			Render::state->angle = 90;
			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_right ) ? 1.f : 0.5f;
			Render::draw_sprite( g_engine->find_asset<Texture_Asset>( "gc_dpad_up" ), Vec2( 130.f, 182.f ) );

			Render::state->angle = 180;
			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_down ) ? 1.f : 0.5f;
			Render::draw_sprite( g_engine->find_asset<Texture_Asset>( "gc_dpad_up" ), Vec2( 102.f, 204.f ) );

			Render::state->angle = 270;
			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_dpad_left ) ? 1.f : 0.5f;
			Render::draw_sprite( g_engine->find_asset<Texture_Asset>( "gc_dpad_up" ), Vec2( 74.f, 182.f ) );

			Render::state->angle = 0;
			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_y ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_y" ), Vec2( 229.f - 16.f, 139.f ) );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_b ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_b" ), Vec2( 261.f - 16.f, 158.f ) );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_a ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_a" ), Vec2( 229.f - 16.f, 179.f ) );

			Render::state->color.a = g_engine->input.is_button_down( input_id::gamepad_button_x ) ? 1.f : 0.5f;
			Render::draw_quad( g_engine->find_asset<Texture_Asset>( "gc_button_x" ), Vec2( 197.f - 16.f, 158.f ) );

			Render::state->color = make_color( Color::black, 0.5f );
			Render::draw_filled_rect( Rect( 0.f, ui_h - 14.f, ui_w, 14.f ) );

			Render::state->align = align::centered;
			Render::state->color = make_color( 4, 1.f );
			Render::draw_string( std::format( "Use controller 'dead zone': {}", use_controller_dead_zone ), Vec2( ui_hw, ui_h - 8.f ) );

			return;
		}

		Render::state->color = make_color( Color::black, 0.5f );
		Render::draw_filled_rect( Rect( 0.f, ui_hh - 7.f, ui_w, 14.f ) );

		Render::state->align = align::centered;
		Render::state->color = make_color( 3, *fader );
		Render::draw_string( "no game controller detected - plug one in!", Vec2( ui_hw, ui_hh ) );
	}
}

bool scene_controller::on_input_pressed( const Input_Event* evt )
{
	if( evt->input_id == input_id::key_d )
	{
		toggle_bool( use_controller_dead_zone );
		return true;
	}

	return false;
}
