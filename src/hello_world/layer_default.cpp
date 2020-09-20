
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force_s = 1.0f;
constexpr float player_move_force_max = 1.25f;
constexpr float player_jump_force = 3.0f;

// ----------------------------------------------------------------------------

constexpr e_collision_layer layer_geo = collision_layer::layer1;
constexpr e_collision_layer layer_player = collision_layer::layer2;
constexpr e_collision_layer layer_player2 = collision_layer::layer3;

// ----------------------------------------------------------------------------

void w_contact_listener::BeginContact( b2Contact* contact )
{
	w_entity_component* ec_a = (w_entity_component*) ( contact->GetFixtureA()->GetBody()->GetUserData().pointer );
	w_entity_component* ec_b = (w_entity_component*) ( contact->GetFixtureB()->GetBody()->GetUserData().pointer );

	log_msg( "ECA : {}", (void*) ec_a );
	log_msg( "ECB : {}", (void*) ec_b );

	assert( ec_a );
	assert( ec_b );

	b2Manifold* manifold = contact->GetManifold();

	b2WorldManifold world_manifold;
	contact->GetWorldManifold( &world_manifold );

	// save contact info for drawing later
	{
		points.clear();

		for( int p = 0 ; p < manifold->pointCount ; ++p )
		{
			points.push_back(
				{
					from_b2d( world_manifold.points[ p ].x ),
					from_b2d( world_manifold.points[ p ].y )
				}
			);
		}
		normal = { manifold->localNormal.x, manifold->localNormal.y };
	}

	log_msg( "A : {}", ec_a->parent_entity->collision_layer );
	log_msg( "B : {}", ec_b->parent_entity->collision_layer );
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

layer_default::layer_default()
{
	draws_completely_solid = true;
}

void layer_default::push()
{
	engine->box2d_world->SetContactListener( &contact_listener );

	world_geo = add_entity<w_entity>();
	world_geo->collision_layer = layer_geo;
	world_geo->draw_debug_info = true;

	float sz = 4.0f;
	world_geo->add_component<ec_b2d_static>()->init_as_chain(
		{
			{ v_window_w - sz * 2, sz },
			{ sz, sz },
			{ sz, v_window_hh + 100 - sz * 2 },
			{ v_window_w - sz * 2, v_window_hh + 100 - sz * 2 }
		}
	);

	world_geo->add_component<ec_b2d_static>()->init_as_line( { sz + 64, v_window_hh + 50 }, { sz + 64 + 150, v_window_hh + 50 } );
	world_geo->add_component<ec_b2d_static>()->init_as_line( { sz + 32, v_window_hh }, { sz + 32 + 50, v_window_hh } );
	world_geo->add_component<ec_b2d_static>()->init_as_line( { sz + 32 + 150, v_window_hh - 25 }, { sz + 32 + 200, v_window_hh - 25 } );
	world_geo->add_component<ec_b2d_static>()->init_as_line( { sz + 100, v_window_hh - 50 }, { sz + 100 + 100, v_window_hh - 50 } );

	for( int x = 0 ; x < 12 ; ++x )
	{
		float xpos = w_random::getf_range( 0.0f, v_window_w );
		float ypos = w_random::getf_range( 64.0f, v_window_h );

		if( w_random::geti_range(0,4) == 45 )
		{
			float sz = w_random::getf_range( 4, 16 );
			world_geo->add_component<ec_b2d_static>()->init_as_circle( { xpos, ypos }, sz );
		}
		else
		{
			float sz = w_random::getf_range( 4, 32 );
			float sz2 = w_random::getf_range( 4, 32 );
			world_geo->add_component<ec_b2d_static>()->init_as_box( { xpos, ypos }, sz, sz2 );
		}
	}

	player = add_entity<w_entity>();
	player->collision_layer = layer_player;
	float xpos = w_random::getf_range( 0.0f, v_window_w );
	xpos = 16.0f;
	player->set_transform( { xpos, 0.0f }, 0, 1 );
	player->add_component<ec_sprite>()->init( "sprite_mario" );
	//auto ecd = player->add_component<ec_b2d_dynamic>()->init_as_box( 8, 8 );
	auto ecd = player->add_component<ec_b2d_dynamic>()->init_as_circle( 8 );
	ecd->body->SetFixedRotation( true );
	ecd->body->GetFixtureList()->SetFriction( 1.0f );
	//ecd->body->GetFixtureList()->SetRestitution( 0.3f );
	//ecd->body->SetGravityScale( 1.5f );

	player2 = add_entity<w_entity>();
	player2->collision_layer = layer_player2;
	player2->set_transform( { v_window_hw, v_window_hh }, 0, 1 );
	player2->add_component<ec_sprite>()->init( "sprite_mario" );
	player2->add_component<ec_b2d_kinematic>()->init_as_circle( 10 );
}

void layer_default::update()
{
	w_layer::update();

	player_on_ground = player->trace_to_closest_point( { 0.0f, 1.0f }, 10.0f, layer_geo, &player_trace_hit );

	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		auto ec = player->get_component<ec_b2d_dynamic>();
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = w_clamp( current.x, -player_move_force_max, player_move_force_max );

		ec->body->SetLinearVelocity( { desired, current.y } );
	}
}

void layer_default::draw()
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

bool layer_default::handle_input_event( const w_input_event* evt )
{
	if( evt->event_id == event_id::input_held )
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

			b2Vec2 current = ec->body->GetLinearVelocity();
			float desired = w_min( current.x + 0.2f, 1.0f );

			ec->body->SetLinearVelocity( { desired, current.y } );
		}

		if( evt->input_id == input_id::key_left )
		{
			auto ec = player->get_component<ec_b2d_dynamic>();

			b2Vec2 current = ec->body->GetLinearVelocity();
			float desired = w_max( current.x - 0.2f, -1.0f );

			ec->body->SetLinearVelocity( { desired, current.y } );
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

	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_space || evt->input_id == input_id::controller_button_a )
		{
			if( player_on_ground )
			{
				auto ec = player->get_component<ec_b2d_dynamic>();
				b2Vec2 current = ec->body->GetLinearVelocity();

				ec->body->SetLinearVelocity( { current.x, -player_jump_force } );
			}
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
