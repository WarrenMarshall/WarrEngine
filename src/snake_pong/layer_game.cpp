
#include "app_header.h"

layer_game::layer_game()
{
	draws_completely_solid = true;
}

void layer_game::push()
{
	w_layer::push();
}

void layer_game::pop()
{
	w_layer::pop();
}

void layer_game::draw()
{
	w_layer::draw();

	//RENDER
	//	->begin()
	//	->push_rgb( w_color::red )
	//	->draw_filled_rectangle( w_rect( 0, 0, v_window_w, v_window_h ) )
	//	->end();
}

void layer_game::draw_ui()
{
	w_layer::draw_ui();
}

void layer_game::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::os );
}
