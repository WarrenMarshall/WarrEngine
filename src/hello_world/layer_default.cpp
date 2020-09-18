
#include "app_header.h"

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	world_geo = add_entity<w_entity>();

	for( int x = 0 ; x < 35 ; ++x )
	{
		float xpos = w_random::getf_range( 0.0f, v_window_w );
		float ypos = w_random::getf_range( 64.0f, v_window_h );

		if( w_random::geti_range(0,2) == 2 )
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
	//player->add_component<ec_b2d_dynamic>()->init_as_box( 8, 10 );
	player->add_component<ec_b2d_dynamic>()->init_as_circle( 10 );

	player2 = add_entity<w_entity>();
	xpos = w_random::getf_range( 0.0f, v_window_w );
	player2->set_transform( { xpos, 0.0f }, 0, 1 );
	player2->add_component<ec_sprite>()->init( "sprite_mario" );
	//player2->add_component<ec_b2d_dynamic>()->init_as_box( 8, 10 );
	player2->add_component<ec_b2d_kinematic>()->init_as_circle( 10 );
}

void layer_default::update()
{
	w_layer::update();
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
			ec->body->SetTransform( { engine->input->mouse_vwindow_pos.x, engine->input->mouse_vwindow_pos.y }, 0 );
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );
			ec->body->SetAwake( true );
		}
	}

	if( evt->event_id == event_id::input_released )
	{
		if( evt->input_id == input_id::controller_button_a )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();

			//ec->body->SetLinearVelocity( b2Vec2( 0, -75 ) );
			ec->body->ApplyLinearImpulseToCenter( b2Vec2( 0, -30000 ), true );
		}

		if( evt->input_id == input_id::key_1 )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();
			ec->body->SetTransform( { engine->input->mouse_vwindow_pos.x, engine->input->mouse_vwindow_pos.y }, 0 );
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );
			ec->body->SetAwake( true );
		}
	}
	return true;
}
