
#include "app_header.h"

// ----------------------------------------------------------------------------

void layer_sound_board_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

	switch( control.tag )
	{
		case H( "button_sfx_01" ):
		{
			a_sound::find( "sfx_01" )->play();
			break;
		}

		case H( "button_sfx_02" ):
		{
			a_sound::find( "sfx_02" )->play();
			break;
		}

		case H( "button_sfx_doom" ):
		{
			a_sound::find( "sfx_doom" )->play();
			break;
		}

		case H( "button_start_music" ):
		{
			a_sound::find( "music_01" )->play();
			break;
		}

		case H( "button_pause_music" ):
		{
			a_sound::find( "music_01" )->pause();
			break;
		}

		case H( "button_stop_music" ):
		{
			a_sound::find( "music_01" )->stop();
			break;
		}
	}
}

// ----------------------------------------------------------------------------

layer_sound_board::layer_sound_board()
{
	draws_completely_solid = true;
}

void layer_sound_board::draw()
{
	w_layer::draw();

	render_state =
	{
		.color = w_color::dark_teal
	};

	w_render::draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, viewport_w, viewport_h ) );

	w_layer::draw();

	w_render::draw_world_axis();
}

void layer_sound_board::draw_ui()
{
	w_layer::draw_ui();

	IMGUI->do_push_button( H( "button_sfx_01" ) )
		->set_text( "Sound : Coin" )
		->set_pos( { 16.0f, 32.0f } )
		->finalize();

	IMGUI->do_push_button( H( "button_sfx_02" ) )
		->set_text( "Sound : Jump" )
		->finalize();

	IMGUI->do_push_button( H( "button_sfx_doom" ) )
		->set_text( "Sound : Doom" )
		->finalize();

	IMGUI->do_divider()->finalize();

	IMGUI->do_push_button( H( "button_start_music" ) )
		->set_text( "Start Music" )
		->finalize();

	IMGUI->do_push_button( H( "button_pause_music" ) )
		->set_text( "Pause" )
		->finalize();

	IMGUI->do_push_button( H( "button_stop_music" ) )
		->set_text( "Stop" )
		->finalize();

	{
		scoped_render_state;

		render_state = {
			.align = align::centered,
			.color = w_color::white,
			.scale = 2.0f
		};

		w_render::draw_string( "Sound Board", { ui_w / 2.0f, 16.0f } );
	}
}

void layer_sound_board::pop()
{
	a_sound::find( "music_01" )->stop();

	w_layer::pop();
}

w_imgui_callback* layer_sound_board::get_imgui_callback()
{
	return &imgui_callback;
}
