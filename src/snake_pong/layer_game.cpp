
#include "app_header.h"

layer_game::layer_game()
{
	is_debug_physics_layer = true;
}

void layer_game::becoming_top_layer()
{
	w_layer::becoming_top_layer();

	engine->window->set_mouse_mode( mouse_mode::custom );
	engine->ui->mouse_cursor = a_cursor::find( "game_cursor_default" );

	//a_sound::find( "gameplay_music" )->play();
}

void layer_game::getting_covered()
{
	//a_sound::find( "gameplay_music" )->stop();
}

void layer_game::update()
{
	w_layer::update();

	power_flicker = engine->random->getb();

	auto player_h = find_entity( H( "player_paddle_h" ) );
	assert( player_h );

	auto player_v = find_entity( H( "player_paddle_v" ) );
	assert( player_v );

	// trace from the center of the world towards the mouse and see if
	// we can hit the outer walls. if so, update the paddle positions.

	w_raycast_closest hit;
	w_vec2 dir = engine->input->mouse_vwindow_pos - w_vec2( v_window_hw, v_window_hh );

	// match the prism rotation to face the same direction as the raycasts
	prism->get_tform()->set_angle( w_vec2::angle_from_dir( dir ) );

	// note : we don't do the raycasting if the mouse is too close the middle of
	// the viewport as this can result in a zero length vector which will crash
	// the raycasting code. this check works around that situation.

	if( dir.get_size_squared() > 4.0f )
	{
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
}

bool layer_game::iir_on_pressed( const w_input_event* evt )
{
	if( evt->input_id == input_id::key_b )
	{
		spawn_ball();
	}

	return false;
}

void layer_game::spawn_ball()
{
	auto e = add_entity<e_ball>();
	e->set_position_deep( w_vec2::zero, false );

	auto dir = w_vec2::dir_from_angle( engine->random->getf_range( 0.f, 360.f ) );
	dir *= e_ball::speed;
	e->get_component<ec_physics>()->get_primary_body()->body->ApplyForceToCenter( dir.to_b2d().as_b2Vec2(), true );
}

void layer_game::new_game()
{
	w_layer::new_game();

	w_entity* e;

	// camera

	e = add_entity<e_camera>();
	e->tag = H( "main_camera" );
	e->get_tform()->set_position( { v_window_hw, v_window_hh } );

	// emitter background

	e = add_entity<w_entity>();
	e->add_component<ec_emitter>()->init( "emitter_game_stars" );

	// world collision geometry

	ec_b2d_static* ecs;
	constexpr float wall_length = 512.0f;

	e = add_entity<w_entity>();
	e->tag = H( "world" );
	e->get_component<ec_physics>()->set_collision_flags( clayer_world_top_bottom, clayer_ball );
	ecs = e->add_component<ec_b2d_static>();
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( +wall_length, -v_window_hh + 4 ), w_vec2( -wall_length, -v_window_hh + 4 ) );
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( -wall_length, v_window_hh - 4 ), w_vec2( +wall_length, v_window_hh - 4 ) );

	e = add_entity<w_entity>();
	e->tag = H( "world" );
	e->get_component<ec_physics>()->set_collision_flags( clayer_world_left_right, clayer_ball );
	ecs = e->add_component<ec_b2d_static>();
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( -v_window_hw + 4, -wall_length ), w_vec2( -v_window_hw + 4, +wall_length ) );
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( v_window_hw - 4, +wall_length ), w_vec2( v_window_hw - 4, -wall_length ) );

	// paddles

	w_rect rc;

	rc = w_rect( -32, -4, 64, 8 );
	e = add_entity<e_paddle>();
	e->tag = H( "player_paddle_h" );
	e->get_component<ec_physics>()->set_collision_flags( clayer_paddle, clayer_ball );
	e->add_component<ec_b2d_kinematic>()->add_fixture_box( "", rc );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::teal * 1.1f, rc );
	e->set_position_deep( w_vec2( -1000, -1000 ), false );

	rc = w_rect( -4, -32, 8, 64 );
	e = add_entity<e_paddle>();
	e->tag = H( "player_paddle_v" );
	e->get_component<ec_physics>()->set_collision_flags( clayer_paddle, clayer_ball );
	e->add_component<ec_b2d_kinematic>()->add_fixture_box( "", rc );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::teal * 1.1f, rc );
	e->set_position_deep( w_vec2( -1000, -1000 ), false );

	// prism

	prism = e = add_entity<w_entity>();
	e->tag = H( "prism" );
	e->add_component<ec_mesh>()->init( "shadow_beam" );
	e->add_component<ec_sprite>()->init( "lightbulb" );
}

void layer_game::draw()
{
	w_layer::draw();

	constexpr float edge_sz = 8.0f;
	w_rect rc = w_rect( -v_window_hw + edge_sz, -v_window_hh + edge_sz, v_window_w - ( edge_sz * 2 ), v_window_h - ( edge_sz * 2 ) );
	w_rect rc2 = w_rect( -v_window_hw + edge_sz - 1.0f, -v_window_hh + edge_sz - 1.0f, v_window_w - ( edge_sz * 2 ) + 2.0f, v_window_h - ( edge_sz * 2 ) + 2.0f );

	RENDER
		->push_rgb( w_color::black )
		->pop()
		->push_rgb( w_color::teal )
		->push_glow( power_flicker ? 0.0f : 1.0f )
		->draw_rectangle( rc )
		->draw_rectangle( rc2 )
		->pop()
		->pop();
}
