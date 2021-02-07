
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
}

void layer_anim_texture::draw()
{
	RS->color = w_color::dark_teal;
	RENDER->draw_tiled( a_texture::find( "engine_tile_background_stripe" ), w_rect( 0.0f, 0.0f, ui_window_w, ui_window_h ) );

	RENDER->push();
	RS->align = align::centered;
	RS->scale = 2.0f;
	RS->color = w_color::white;
	RENDER->draw_string( "Animated Textures", { ui_window_w / 2.0f, 16.0f } );
	RENDER->pop();

	w_layer::draw();
}

void layer_anim_texture::draw_ui()
{
	w_layer::draw_ui();

	IMGUI
		->do_panel()
		->set_text( "Simple" )
		->set_position( { 16.0f, 32.0f } )
		->set_size( { 64.0f, 48.0f} )
		->finalize();

	IMGUI
		->do_panel()
		->set_text( "Offsets" )
		->set_position( { 96.0f, 32.0f } )
		->set_size( { 128.0f, 48.0f } )
		->finalize();

	RS->color = w_color::white;
	RENDER->draw_sprite( animtex_01->get_texture( 0.0f ), { 48.0f, 64.0f } );
	RENDER->draw_sprite( animtex_coin_01->get_texture( 0.0f ), { 128.0f, 64.0f } );
	RENDER->draw_sprite( animtex_coin_01->get_texture( 0.3f ), { 160.0f, 64.0f } );
	RENDER->draw_sprite( animtex_coin_01->get_texture( 0.6f ), { 192.0f, 64.0f } );
}

w_imgui_callback* layer_anim_texture::get_imgui_callback()
{
	return &imgui_callback;
}
