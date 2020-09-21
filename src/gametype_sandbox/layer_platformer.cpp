
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force_s = 7.5f;
constexpr float player_move_force_max = 1.5f;
constexpr float player_jump_force = 3.0f;

// ----------------------------------------------------------------------------

constexpr e_collision_layer clayer_world = collision_layer::bit2;
constexpr e_collision_layer clayer_player = collision_layer::bit3;
constexpr e_collision_layer clayer_player2 = collision_layer::bit4;

// ----------------------------------------------------------------------------

void w_contact_listener::BeginContact( b2Contact* contact )
{
	w_entity_component* ec_a = (w_entity_component*) ( contact->GetFixtureA()->GetBody()->GetUserData().pointer );
	w_entity_component* ec_b = (w_entity_component*) ( contact->GetFixtureB()->GetBody()->GetUserData().pointer );


}

void w_contact_listener::EndContact( b2Contact* contact )
{
}

void w_contact_listener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{

}

void w_contact_listener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{

}

// ----------------------------------------------------------------------------

layer_platformer::layer_platformer()
{
	draws_completely_solid = true;
}

void layer_platformer::push()
{
	engine->box2d_world->SetContactListener( &contact_listener );

	ec_b2d_body* ec = nullptr;

	world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_player;
	world_geo->draw_debug_info = true;
	ec = world_geo->add_component<ec_b2d_static>();
	{
		auto fixture = ec->add_fixture_chain(
			{
				{ v_window_w - 8.0f, 4.0f },
				{ 4.0f, 4.0f },
				{ 4.0f, v_window_hh + 100 - 8.0f },
				{ v_window_w - 8.0f, v_window_hh + 100 - 8.0f }
			},
			w_vec2::zero
		);

		ec->add_fixture_line( { 4.0f + 64, v_window_hh + 55 }, { 4.0f + 64 + 150, v_window_hh + 55 }, w_vec2::zero );
		ec->add_fixture_line( { 4.0f + 32, v_window_hh + 20 }, { 4.0f + 32 + 50, v_window_hh + 20 }, w_vec2::zero );
		ec->add_fixture_line( { 4.0f + 32 + 150, v_window_hh }, { 4.0f + 32 + 200, v_window_hh }, w_vec2::zero );
		ec->add_fixture_line( { 100.0f, v_window_hh - 20 }, { 200.f, v_window_hh - 20 }, w_vec2::zero );
	}

	for( int x = 0 ; x < 12 ; ++x )
	{
		float xpos = w_random::getf_range( 0.0f, v_window_w );
		float ypos = w_random::getf_range( 64.0f, v_window_h );

		if( w_random::geti_range(0,4) == 0 )
		{
			float sz = w_random::getf_range( 4, 16 );
			ec->add_fixture_circle( sz, { xpos, ypos } );
		}
		else
		{
			float sz = w_random::getf_range( 4, 32 );
			float sz2 = w_random::getf_range( 4, 32 );
			ec->add_fixture_box( sz, sz2, { xpos, ypos } );
		}
	}

	// ----------------------------------------------------------------------------

	player = add_entity<w_entity>();
	player->collision_layer = clayer_player;
	player->collides_with = clayer_world;
	player->draw_debug_info = true;
	player->set_transform( { 32.0f, 0.0f }, 0, 1 );

	//player->add_component<ec_sprite>()->init( "sprite_mario" );

	ec = player->add_component<ec_b2d_dynamic>();
	ec->body->SetFixedRotation( true );
	ec->add_fixture_circle( 8, w_vec2::zero );
	b2Fixture* f = ec->add_fixture_box( 4, 2, w_vec2( 0, 8 ) );
	f->SetSensor( true );

	// ----------------------------------------------------------------------------

	/*
	player2 = add_entity<w_entity>();
	player2->collision_layer = clayer_player2;
	player2->collides_with = clayer_player;
	player2->set_transform( { v_window_hw, v_window_hh }, 0, 1 );
	player2->add_component<ec_sprite>()->init( "sprite_mario" );
	player2->add_component<ec_b2d_kinematic>()->init_as_circle( 10 );
	*/
}

void layer_platformer::update()
{
	w_layer::update();

	player_on_ground = player->trace_to_closest_point( { 0.0f, 1.0f }, 10.0f, clayer_world, &player_trace_hit );

	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		if( !player_on_ground )
		{
			// remove air control
		}

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = w_clamp( current.x, -player_move_force_max, player_move_force_max );

		ec->body->SetLinearVelocity( { desired, current.y } );
	}
}

void layer_platformer::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgb( { 1.0f, 0.0f, 0.0f } );

	for( auto& v : contact_listener.points )
	{
		RENDER->draw_point( v );
	}

	if( contact_listener.points.size() )
	{
		RENDER->draw_line(
			contact_listener.points[ 0 ],
			contact_listener.points[ 0 ] + ( contact_listener.normal * 16.0f )
		);
	}

	RENDER
		->push_rgb( { 0.5f, 0.5f, 1.0f } )
		->draw_point( player_trace_hit );

	RENDER->end();
}

bool layer_platformer::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_held )
	{
		if( evt->input_id == input_id::key_2 )
		{
			auto ec = player2->get_component<ec_b2d_body>( component_type::b2d_kinematic );
			ec->body->SetTransform( { to_b2d( engine->input->mouse_vwindow_pos.x ), to_b2d( engine->input->mouse_vwindow_pos.y ) }, 0 );
			ec->body->SetLinearVelocity( { 0, 0 } );
			ec->body->SetAngularVelocity( 0 );
			ec->body->SetAwake( true );
		}

		if( evt->input_id == input_id::key_right )
		{
			auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );

			b2Vec2 current = ec->body->GetLinearVelocity();
			float desired = w_min( current.x + 0.2f, 1.0f );

			ec->body->SetLinearVelocity( { desired, current.y } );
		}

		if( evt->input_id == input_id::key_left )
		{
			auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );

			b2Vec2 current = ec->body->GetLinearVelocity();
			float desired = w_max( current.x - 0.2f, -1.0f );

			ec->body->SetLinearVelocity( { desired, current.y } );
		}

		if( evt->input_id == input_id::key_up )
		{
			auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
			ec->body->ApplyTorque( to_b2d(20.f), true );
		}

		if( evt->input_id == input_id::key_down )
		{
			auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
			ec->body->ApplyAngularImpulse( to_b2d( 1.f ), true );
		}
	}

	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_space || evt->input_id == input_id::controller_button_a )
		{
			if( player_on_ground )
			{
				auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
				b2Vec2 current = ec->body->GetLinearVelocity();

				ec->body->SetLinearVelocity( { current.x, -player_jump_force } );
			}
		}

		if( evt->input_id == input_id::key_1 )
		{
			auto ecs = player->get_components<ec_b2d_body>( component_type::b2d_dynamic );

			for( auto ec : ecs )
			{
				ec->body->SetTransform( { to_b2d( engine->input->mouse_vwindow_pos.x ), to_b2d( engine->input->mouse_vwindow_pos.y ) }, 0 );
				ec->body->SetLinearVelocity( { 0, 0 } );
				ec->body->SetAngularVelocity( 0 );
				ec->body->SetAwake( true );
			}
		}
	}
	return true;
}
