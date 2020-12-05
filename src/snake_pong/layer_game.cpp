
#include "app_header.h"

layer_game::layer_game()
{
	//draws_completely_solid = true;
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

	engine->window->set_mouse_mode( mouse_mode::locked );
}

w_camera* layer_game::get_camera()
{
	return find_from_tag<w_camera>( "main_camera" );
}

void layer_game::update()
{
	w_layer::update();

	auto player_h = find_from_tag<w_entity>( "player_paddle_h" );
	assert( player_h );

	auto player_v = find_from_tag<w_entity>( "player_paddle_v" );
	assert( player_v );

	// trace from the center of the world towards the mouse and see if
	// we can hit the outer walls. if so, update the paddle positions.

	w_raycast_closest hit;
	w_vec2 n = ( engine->input->mouse_vwindow_pos - w_vec2( v_window_hw, v_window_hh ) ).normalize();

	// horizontal

	w_physics_query::trace_closest( w_vec2( 0, 0 ), n, 2000.f, clayer_world_top_bottom, &hit );

	if( hit.hit_something )
	{
		player_h->set_position_deep( hit.result.pos, b_reset_velocity( true ) );
	}

	// vertical

	hit = {};
	w_physics_query::trace_closest( w_vec2( 0, 0 ), n, 2000.f, clayer_world_left_right, &hit );

	if( hit.hit_something )
	{
		player_v->set_position_deep( hit.result.pos, b_reset_velocity( true ) );
	}
}
