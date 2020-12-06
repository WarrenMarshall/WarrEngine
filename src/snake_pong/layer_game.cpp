
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

bool layer_game::iir_on_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::key_b )
	{
		spawn_ball();
	}

	if( evt->input_id == input_id::key_r )
	{
		auto ball = find_from_tag<e_ball>( "ball" );
		ball->set_position_deep( w_vec2::zero, false );

		return true;
	}

	return false;
}

void layer_game::spawn_ball()
{
	auto e = add_entity<e_ball>();
	e->set_tag( "ball" );
	e->set_collision( clayer_ball, clayer_player_h | clayer_player_v | clayer_ball );
	auto ecd = e->add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	ecd->add_fixture_circle( "", w_vec2::zero, e_ball::radius );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::pal( 4 ), w_rect(-e_ball::radius, -e_ball::radius, e_ball::radius * 2.f, e_ball::radius * 2.f ) );
	e->set_position_deep( w_vec2::zero, false );

	auto dir = w_vec2::dir_from_angle( engine->random->getf_range( 0.f, 360.f ) );
	dir *= e_ball::speed;
	e->phys_get_primary_body()->body->ApplyForceToCenter( dir.to_b2d().as_b2Vec2(), true );
}
