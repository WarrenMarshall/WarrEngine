
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force_s = 7.5f;
constexpr float player_move_force_max = 1.25f;
constexpr float player_jump_force = 3.0f;
constexpr float player_radius = 8.0f;

// ----------------------------------------------------------------------------

constexpr unsigned IDF_None = 0;
constexpr unsigned IDF_PlayerFootSensor = 1;
constexpr unsigned IDF_World = 2;

// ----------------------------------------------------------------------------

constexpr e_collision_layer clayer_world = collision_layer::bit2;
constexpr e_collision_layer clayer_player = collision_layer::bit3;
constexpr e_collision_layer clayer_player2 = collision_layer::bit4;

// ----------------------------------------------------------------------------

constexpr bool ids_match( unsigned a_id, unsigned b_id, unsigned id_1, unsigned id_2 )
{
	return( ( a_id == id_1 && b_id == id_2 ) || ( a_id == id_2 && b_id == id_1 ) );
}

void w_contact_listener::BeginContact( b2Contact* contact )
{
	unsigned a_id = (unsigned) contact->GetFixtureA()->GetUserData().pointer;
	unsigned b_id = (unsigned) contact->GetFixtureB()->GetUserData().pointer;

	if( ids_match( a_id, b_id, IDF_PlayerFootSensor, IDF_World ) )
	{
		layer_platformer* layer = ( layer_platformer * )engine->layer_mgr->get_top();
		layer->player_on_ground++;
	}
}

void w_contact_listener::EndContact( b2Contact* contact )
{
	unsigned a_id = (unsigned) contact->GetFixtureA()->GetUserData().pointer;
	unsigned b_id = (unsigned) contact->GetFixtureB()->GetUserData().pointer;

	if( ids_match( a_id, b_id, IDF_PlayerFootSensor, IDF_World ) )
	{
		layer_platformer* layer = (layer_platformer*) engine->layer_mgr->get_top();
		layer->player_on_ground--;
	}
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
		// bounding box for world

		ec->add_fixture_chain(
			IDF_World,
			w_vec2::zero,
			{
				{ 4.0f, 4.0f },
				{ v_window_w - 8.0f, 4.0f },
				{ v_window_w - 8.0f, v_window_h - 8.0f },
				{ 4.0f, v_window_h - 8.0f }
			}
		);

		// rando lines running vertically

		for( int y = 16 ; y < v_window_h ; y += 30 )
		{
			float xpos = w_random::getf_range( 0, v_window_w );
			float ypos = (float) y;
			float w = w_random::getf_range( 50, 200 );
			ec->add_fixture_line( IDF_World, w_vec2::zero, { xpos, ypos }, { xpos + w, ypos } );
		}
	}

	// ----------------------------------------------------------------------------

	player = add_entity<w_entity>();
	player->collision_layer = clayer_player;
	player->collides_with = clayer_world;
	//player->draw_debug_info = true;
	player->set_transform( { 32.0f, 0.0f }, 0, 1 );

	player->add_component<ec_sprite>()->init( "sprite_mario" );

	ec = player->add_component<ec_b2d_dynamic>();
	ec->body->SetFixedRotation( true );

	b2Fixture* f = ec->add_fixture_circle( IDF_None, w_vec2::zero, player_radius );

	f = ec->add_fixture_box( IDF_PlayerFootSensor, { 0.0f, player_radius }, 10.0f, 4.0f );
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

	//player_on_ground = player->trace_to_closest_point( { 0.0f, 1.0f }, 10.0f, clayer_world, &player_trace_hit );

	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		if( !player_on_ground )
		{
			left_stick.x *= 0.5f;
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
				w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

				auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
				b2Vec2 current = ec->body->GetLinearVelocity();

				float dir_modifier = 1.0f;
				if( !fequals( left_stick.y, 0.0f ) && left_stick.y > 0.0f )
				{
					dir_modifier = -0.5f;
					auto pos = ec->body->GetPosition();
					ec->body->SetTransform( { pos.x, pos.y + to_b2d( player_radius + 1.0f ) }, 0.0f );
				}
				ec->body->SetLinearVelocity( { current.x, (-player_jump_force) * dir_modifier } );
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
