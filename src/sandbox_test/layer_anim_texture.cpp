
#include "app_header.h"

// ----------------------------------------------------------------------------

void layer_anim_texture_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

// 	switch( control.tag )
// 	{
// 		case H( "push_button_01" ):
// 		{
// 			UI->show_msg_box( "You clicked the button!" );
// 		}
// 		break;
// 	}
}

void layer_anim_texture_callback::on_motion( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_motion( control, result );

	auto layer = static_cast<layer_anim_texture*>( IMGUI->current_layer );

// 	switch( control.tag )
// 	{
// 		case H( "slider_01" ):
// 		{
// 		}
// 		break;
// 	}
}

// ----------------------------------------------------------------------------

layer_anim_texture::layer_anim_texture()
{
	draws_completely_solid = true;
}

void layer_anim_texture::push()
{
	animtex_01 = a_anim_texture::find( "anim_player_run" );
	animtex_coin_01 = a_anim_texture::find( "anim_coin" );

	engine->window->set_mouse_mode( mouse_mode::os );

	{
		add_camera();
	}
}

void layer_anim_texture::draw()
{
	render_state =
	{
		.color = w_color::dark_teal
	};

	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, v_window_w, v_window_h ) );

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_anim_texture::draw_ui()
{
	w_layer::draw_ui();

	{
		scoped_imgui_location_offset( w_vec2( 64.0f, 32.0f ) );

		IMGUI->do_panel();
		IMGUI->set_text( "Simple" );
		IMGUI->set_size( { 64.0f, 48.0f } );
		IMGUI->finalize();

		IMGUI
			->do_panel()
			->set_text( "Offsets" )
			->set_pos( imgui_flow::right )
			->set_size( { 128.0f, 48.0f } )
			->finalize();

		render_state =
		{
			.color = w_color::white
		};

		w_render::draw_sprite( animtex_01->get_texture( 0.0f ), w_vec2( 32.0f, 32.0f ) + IMGUI->location_offset );
		w_render::draw_sprite( animtex_coin_01->get_texture( 0.0f ), w_vec2( 96.0f, 32.0f ) + IMGUI->location_offset );
		w_render::draw_sprite( animtex_coin_01->get_texture( 0.3f ), w_vec2( 128.0f, 32.0f ) + IMGUI->location_offset );
		w_render::draw_sprite( animtex_coin_01->get_texture( 0.6f ), w_vec2( 160.0f, 32.0f ) + IMGUI->location_offset );
	}

	{
		scoped_render_state;

		render_state =
		{
			.align = align::centered,
			.color = w_color::white,
			.scale = 2.0f
		};

		w_render::draw_string( "Animated Textures", { ui_window_w / 2.0f, 16.0f } );

		render_state =
		{
			.align = align::hcenter,
			.color = w_color::light_grey,
			.scale = 1.0f
		};

		w_render::draw_string( "R_DRAG / M_DRAG - move/rotate camera", w_pos( ui_window_hw, 200.0f ) );
	}
}

w_imgui_callback* layer_anim_texture::get_imgui_callback()
{
	return &imgui_callback;
}

bool layer_anim_texture::on_input_motion( const w_input_event* evt )
{
	if( evt->input_id == input_id::mouse )
	{
		if( INPUT->get_button_state( input_id::mouse_button_right ) == button_state::held )
		{
			get_camera()->get_transform()->add_pos_delta( w_coord::window_to_virtual( evt->delta ) );

			return true;
		}

		if( INPUT->get_button_state( input_id::mouse_button_middle ) == button_state::held )
		{
			get_camera()->get_transform()->add_angle_delta( w_coord::window_to_virtual( evt->delta ).x );

			return true;
		}
	}

	return false;
}
