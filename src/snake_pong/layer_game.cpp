
#include "app_header.h"

layer_game::layer_game()
{
	is_debug_physics_layer = true;
}

void layer_game::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::os );
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

	// this "dir" dance is here to handle the case where the user puts the mouse
	// in the dead center of the viewport. This results in a zero length vector which
	// causes a crash in the raycast and other bad things.

	w_vec2 dir = engine->input->mouse_vwindow_pos - w_vec2( v_window_hw, v_window_hh );

	if( ( engine->input->mouse_vwindow_pos - w_vec2( v_window_hw , v_window_hh ) ).get_size_squared() < 4.0f )
	{
		dir = w_vec2( 0.0f, -1.0f );
	}

	w_vec2 n = ( dir ).normalize();

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
	e->set_position_deep( w_vec2::zero, false );

	auto dir = w_vec2::dir_from_angle( engine->random->getf_range( 0.f, 360.f ) );
	dir *= e_ball::speed;
	e->phys_get_primary_body()->body->ApplyForceToCenter( dir.to_b2d().as_b2Vec2(), true );
}

void layer_game::new_game()
{
	w_layer::new_game();

	w_entity* e;

	// camera

	e = add_entity<e_camera>();
	e->set_tag( "main_camera" );
	e->it_set_position( { v_window_hw, v_window_hh } );

	// world collision geometry

	ec_b2d_static* ecs;
	constexpr float wall_length = 512.0f;

	e = add_entity<w_entity>();
	e->set_tag( "world" );
	e->set_collision( clayer_world_top_bottom, clayer_ball );
	ecs = e->add_component<ec_b2d_static>();
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( +wall_length, -v_window_hh + 4 ), w_vec2( -wall_length, -v_window_hh + 4 ) );
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( -wall_length, v_window_hh - 4 ), w_vec2( +wall_length, v_window_hh - 4 ) );

	e = add_entity<w_entity>();
	e->set_tag( "world" );
	e->set_collision( clayer_world_left_right, clayer_ball );
	ecs = e->add_component<ec_b2d_static>();
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( -v_window_hw + 4, -wall_length ), w_vec2( -v_window_hw + 4, +wall_length ) );
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( v_window_hw - 4, +wall_length ), w_vec2( v_window_hw - 4, -wall_length ) );

	// paddles

	w_rect rc;

	rc = w_rect( -32, -4, 64, 8 );
	e = add_entity<e_paddle>();
	e->set_tag( "player_paddle_h" );
	e->set_collision( clayer_paddle, clayer_ball );
	e->add_component<ec_b2d_kinematic>()->add_fixture_box( "", rc );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::pal( 4 ) * 1.1f, rc );
	e->set_position_deep( w_vec2( -1000, -1000 ), false );

	rc = w_rect( -4, -32, 8, 64 );
	e = add_entity<e_paddle>();
	e->set_tag( "player_paddle_v" );
	e->set_collision( clayer_paddle, clayer_ball );
	e->add_component<ec_b2d_kinematic>()->add_fixture_box( "", rc );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::pal( 4 ) * 1.1f, rc );
	e->set_position_deep( w_vec2( -1000, -1000 ), false );
}
