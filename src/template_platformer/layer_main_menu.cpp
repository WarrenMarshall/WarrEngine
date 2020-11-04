
#include "app_header.h"

layer_main_menu::layer_main_menu()
{
	draws_completely_solid = true;
}

void layer_main_menu::push()
{
	w_layer::push();
	//game->music_main_menu->play();
}

void layer_main_menu::pop()
{
	w_layer::pop();
	//game->music_main_menu->stop();
}

void layer_main_menu::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgb( w_color::light_blue )
		->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) )
		->end();
}

void layer_main_menu::draw_ui()
{
	w_layer::draw_ui();

	RENDER
		->begin()
		->push_scale( { 2.0f, 2.0f } )
		->push_align( align::hcenter )
		->push_rgb( w_color::white )
		->draw_string( engine->pixel_font, "Platformer Template", w_rect( v_window_hw, 32 ) )
		->end();

	float xpos = 64;
	float ypos = 75;

	if( IMGUI->init_push_button()
		->set_label( "PLAY" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( { xpos, ypos, v_window_w - xpos * 2, 24 } )
		->finalize()
		->was_left_clicked() )
	{
		game->new_game();
		engine->layer_mgr->pop();
		engine->layer_mgr->push<platformer_layer>();
		game->load_level( engine->fs->prepend_data_path( "levels/level_0.tmx" ) );
	}

	ypos += 32;
	if( IMGUI->init_push_button()
		->set_label( "Exit" )
		->set_slice_def( a_9slice_def::find( "sd_push_button" ) )
		->set_rect( { xpos, ypos, v_window_w - xpos * 2, 24 } )
		->finalize()
		->was_left_clicked() )
	{
		engine->is_running = false;
	}

	ypos += 24;
	RENDER
		->push_align( align::hcenter )
		->push_rgb( w_color::light_grey )
		->draw_string( engine->pixel_font, "-- Controls --", w_rect( v_window_hw, ypos += 16 ) )
		->push_rgb( w_color::dark_grey )
		->draw_string( engine->pixel_font, "F11 - Toggle Fullscreen", w_rect( v_window_hw, ypos += 16 ) )
		->draw_string( engine->pixel_font, "Controller - Move", w_rect( v_window_hw, ypos += 8 ) )
		->draw_string( engine->pixel_font, "A-Button - Jump", w_rect( v_window_hw, ypos += 8 ) )
		->draw_string( engine->pixel_font, "DOWN + A-Button - Drop Down", w_rect( v_window_hw, ypos += 8 ) )
		->draw_string( engine->pixel_font, "'N' - Spawn Coins From Sky", w_rect( v_window_hw, ypos += 16 ) );
}

void layer_main_menu::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::os );
}
