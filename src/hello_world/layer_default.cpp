
#include "app_header.h"

constexpr float player_move_force = 50.0f;
constexpr float player_jump_force = -10.0f;

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	world_geo = add_entity<w_entity>();

	float sz = 4.0f;
	world_geo->add_component<ec_b2d_static>()->init_as_chain(
		{
			{ v_window_w - sz * 2, sz },
			{ sz, sz },
			{ sz, v_window_hh + 100 - sz * 2 },
			{ v_window_w - sz * 2, v_window_hh + 100 - sz * 2 }
		}
	);

	for( int x = 0 ; x < 20 ; ++x )
	{
		float xpos = w_random::getf_range( 0.0f, v_window_w );
		float ypos = w_random::getf_range( 64.0f, v_window_h );

		if( w_random::geti_range(0,2) == 1 )
		{
			float sz = w_random::getf_range( 4, 32 );
			float sz2 = w_random::getf_range( 4, 32 );
			world_geo->add_component<ec_b2d_static>()->init_as_box( { xpos, ypos }, sz, sz2 );
		}
		else
		{
			float sz = w_random::getf_range( 4, 16 );
			world_geo->add_component<ec_b2d_static>()->init_as_circle( { xpos, ypos }, sz );
		}
	}

	player = add_entity<w_entity>();
	float xpos = w_random::getf_range( 0.0f, v_window_w );
	player->set_transform( { xpos, 0.0f }, 0, 1 );
	player->add_component<ec_sprite>()->init( "sprite_mario" );
	//auto ecd = player->add_component<ec_b2d_dynamic>()->init_as_box( 8, 10 );
	auto ecd = player->add_component<ec_b2d_dynamic>()->init_as_circle( 10 );
	ecd->body->SetFixedRotation( true );

	player2 = add_entity<w_entity>();
	xpos = w_random::getf_range( 0.0f, v_window_w );
	player2->set_transform( { xpos, 0.0f }, 0, 1 );
	player2->add_component<ec_sprite>()->init( "sprite_mario" );
	player2->add_component<ec_b2d_kinematic>()->init_as_circle( 10 );
}

void layer_default::update()
{
	w_layer::update();

	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		auto ec = player->get_component<ec_b2d_dynamic>();
		ec->body->ApplyForceToCenter( b2Vec2( to_b2d( player_move_force * left_stick.x ), 0 ), true );
	}
}

void layer_default::draw()
{
	w_layer::draw();
}

bool layer_default::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_2 )
		{
			auto ec = player2->get_component<ec_b2d_kinematic>();
			ec->body->SetTransform( { to_b2d( engine->input->mouse_vwindow_pos.x ), to_b2d( engine->input->mouse_vwindow_pos.y ) }, 0 );
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );
			ec->body->SetAwake( true );
		}

		if( evt->input_id == input_id::key_right )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();
			ec->body->ApplyForceToCenter( b2Vec2( to_b2d( player_move_force ), 0 ), true );
		}

		if( evt->input_id == input_id::key_left )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();
			ec->body->ApplyForceToCenter( b2Vec2( to_b2d( -player_move_force ), 0 ), true );
		}

		if( evt->input_id == input_id::key_up )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();
			ec->body->ApplyTorque( to_b2d(20.f), true );
		}

		if( evt->input_id == input_id::key_down )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();
			ec->body->ApplyAngularImpulse( to_b2d( 1.f ), true );
		}
	}

	if( evt->event_id == event_id::input_released )
	{
		if( evt->input_id == input_id::key_space || evt->input_id == input_id::controller_button_a )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();
			ec->body->ApplyLinearImpulseToCenter( b2Vec2( 0, to_b2d( player_jump_force ) ), true );
		}

		if( evt->input_id == input_id::key_1 )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();
			ec->body->SetTransform( { to_b2d( engine->input->mouse_vwindow_pos.x ), to_b2d( engine->input->mouse_vwindow_pos.y ) }, 0 );
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );
			ec->body->SetAwake( true );
		}
	}
	return true;
}
