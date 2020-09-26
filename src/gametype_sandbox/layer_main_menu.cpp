
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
		->push_rgb( w_color::light_blue )
		->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) );

	float xpos = 64;
	float ypos = 75;

	if( im_left_clicked( UI->im_active( "Platformer", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		engine->layer_mgr->push<layer_platformer>();
	}

	ypos += 28;
	if( im_left_clicked( UI->im_active( "Pong", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		engine->layer_mgr->push<layer_pong>();
	}

	ypos += 28;
	if( im_left_clicked( UI->im_active( "Twin Stick Shooter", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		engine->layer_mgr->push<layer_platformer>();
	}

	ypos += 28;
	if( im_left_clicked( UI->im_active( "Time Pilot", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		engine->layer_mgr->push<layer_platformer>();
	}

	ypos += 48;
	if( im_left_clicked( UI->im_active( "Exit", w_rect( xpos, ypos, v_window_w - xpos * 2, 24 ), *UI->theme->default_button_style ) ) )
	{
		engine->is_running = false;
	}
}

