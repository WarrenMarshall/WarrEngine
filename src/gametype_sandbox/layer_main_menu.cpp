
#include "app_header.h"

layer_main_menu::layer_main_menu()
{
	draws_completely_solid = true;
}

void layer_main_menu::push()
{
}

void layer_main_menu::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgb( w_color::light_blue )
		->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) )
		->push_scale( 2.0f )
		->push_align( align::hcenter )
		->push_rgb( w_color::white )
		->draw_string( engine->pixel_font, "gametype sandbox", w_rect( v_window_hw, 32 ) )
		->end();

#if 0
	float xpos = 64;
	float ypos = 75;

	if( im_left_clicked( UI->im_active( "Platformer", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		game->new_game();
		engine->layer_mgr->push<platformer_layer>();
	}

	ypos += 28;
	if( im_left_clicked( UI->im_active( "Break Out", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		game->new_game();
		engine->layer_mgr->push<breakout_layer>();
	}

	ypos += 28;
	if( im_left_clicked( UI->im_active( "Twin Stick Shooter", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		game->new_game();
		engine->layer_mgr->push<twinstick_layer>();
	}

	ypos += 28;
	if( im_left_clicked( UI->im_active( "Tile Based RPG", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		game->new_game();
		engine->layer_mgr->push<platformer_layer>();
	}

	ypos += 48;
	if( im_left_clicked( UI->im_active( "Exit", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		engine->is_running = false;
	}
#endif

}

void layer_main_menu::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::os );
}

bool layer_main_menu::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_t )
		{
			game->new_game();
			engine->layer_mgr->push<twinstick_layer>();
			return true;
		}
	}

	return false;
}
