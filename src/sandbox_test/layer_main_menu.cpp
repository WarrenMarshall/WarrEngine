
#include "app_header.h"

// ----------------------------------------------------------------------------

void layer_main_menu_ui_callback::on_left_clicked( const w_imgui_control& control, const w_imgui_result& result )
{
	w_imgui_callback::on_left_clicked( control, result );

	switch( control.tag )
	{
		case H( "button_ui" ):
		{
			LAYER_MGR->pop();
			LAYER_MGR->push<layer_ui>();

			break;
		}

		case H( "button_anim_texture" ):
		{
			LAYER_MGR->pop();
			LAYER_MGR->push<layer_anim_texture>();

			break;
		}

		case H( "button_particles" ):
		{
			LAYER_MGR->pop();
			LAYER_MGR->push<layer_particles>();

			break;
		}

		case H( "button_meshes" ):
		{
			LAYER_MGR->pop();
			LAYER_MGR->push<layer_meshes>();

			break;
		}

		case H( "button_entity_picking" ):
		{
			LAYER_MGR->pop();
			LAYER_MGR->push<layer_entity_picking>();

			break;
		}

		case H( "button_sound_board" ):
		{
			LAYER_MGR->pop();
			LAYER_MGR->push<layer_sound_board>();

			break;
		}
	}
}

// ----------------------------------------------------------------------------

layer_main_menu::layer_main_menu()
{
	draws_completely_solid = true;
}

void layer_main_menu::push()
{
	gradient = a_texture::find( "background_gradient" );

	engine->window->set_mouse_mode( mouse_mode::os );
}

void layer_main_menu::draw()
{
	w_layer::draw();

	RENDER
		->draw( gradient, w_rect( 0.0f, 0.0f, v_window_w, v_window_h ) );
}

void layer_main_menu::draw_ui()
{
	w_layer::draw_ui();

	IMGUI->do_panel( H( "main_panel" ) )
		->set_text( "Test Cases" )
		->set_pos( { 4.0f, 4.0f } )
		->set_size( { ui_window_w - 8, ui_window_h - 8 } )
		->finalize();

	IMGUI->do_push_button( H( "button_ui" ) )
		->set_text( "UI Controls" )
		->set_pos( imgui_flow::last_crc_topleft )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_anim_texture" ) )
		->set_text( "Anim Texture" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_particles" ) )
		->set_text( "Particles" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_meshes" ) )
		->set_text( "Meshes" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_entity_picking" ) )
		->set_text( "Entity Picking" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_sound_board" ) )
		->set_text( "Sound Board" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	/*
	IMGUI->do_push_button( H( "button_box2d" ) )
		->set_text( "Box2D" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_particles" ) )
		->set_text( "Particles" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_sounds" ) )
		->set_text( "Sound FX" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();

	IMGUI->do_push_button( H( "button_game_controller" ) )
		->set_text( "Game Controller" )
		->set_size( { 100.0f, w_vec2::ignore } )
		->finalize();
	*/
}

w_imgui_callback* layer_main_menu::get_imgui_callback()
{
	return &imgui_callback;
}
