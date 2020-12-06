
#include "app_header.h"

snake_pong::snake_pong( const char* name )
	: w_game( name )
{
}

void snake_pong::init()
{
	w_game::init();

	engine->layer_mgr->push<layer_background>();
	engine->layer_mgr->push<layer_main_menu>();
}

void snake_pong::new_game()
{
	w_game::new_game();

	engine->box2d_world->SetGravity( w_vec2( 0.f, 0.f ).as_b2Vec2() );

	auto layer = engine->layer_mgr->get_top();

	w_entity* e;

	// camera

	e = layer->add_entity<w_camera>();
	e->set_tag( "main_camera" );

	// world collision geometry

	ec_b2d_static* ecs;
	constexpr float wall_length = 2000.0f;

	e = layer->add_entity<w_entity>();
	ecs = e->add_component<ec_b2d_static>();
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( +wall_length, -v_window_hh + 4 ), w_vec2( -wall_length, -v_window_hh + 4 ) );
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( -wall_length, v_window_hh - 4 ), w_vec2( +wall_length, v_window_hh - 4 ) );
	e->collision_layer = clayer_world_top_bottom;

	e = layer->add_entity<w_entity>();
	ecs = e->add_component<ec_b2d_static>();
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( -v_window_hw + 4, -wall_length ), w_vec2( -v_window_hw + 4, +wall_length ) );
	ecs->add_fixture_line( "", w_vec2::zero, w_vec2( v_window_hw - 4, +wall_length ), w_vec2( v_window_hw - 4, -wall_length ) );
	e->collision_layer = clayer_world_left_right;

	// paddles

	w_rect rc;

	rc = w_rect( -32, -4, 64, 8 );
	e = layer->add_entity<w_entity>();
	e->set_tag( "player_paddle_h" );
	e->collision_layer = clayer_player_h;
	e->add_component<ec_b2d_kinematic>()->add_fixture_box( "", rc );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::pal( 4 ), rc );
	e->set_position_deep( w_vec2( -1000, -1000 ), false );

	rc = w_rect( -4, -32, 8, 64 );
	e = layer->add_entity<w_entity>();
	e->set_tag( "player_paddle_v" );
	e->collision_layer = clayer_player_v;
	e->add_component<ec_b2d_kinematic>()->add_fixture_box( "", rc );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::filled_rectangle, w_color::pal( 4 ), rc );
	e->set_position_deep( w_vec2( -1000, -1000 ), false );

	// ball

	e = layer->add_entity<e_ball>();
	e->set_tag( "ball" );
	e->collision_layer = clayer_ball;
	auto ecd = e->add_component<ec_b2d_dynamic>();
	ecd->is_primary_body = true;
	ecd->add_fixture_circle( "", w_vec2::zero, 8.f );
	e->add_component<ec_primitive_shape>()->init( primitive_shape::circle, w_color::pal( 4 ), 8.f );
	e->set_position_deep( w_vec2::zero, false );

	e->set_angle_deep( engine->random->getf_range( 0.f, 360.f ) );
	e->phys_set_restitution( 1.0f );
}
