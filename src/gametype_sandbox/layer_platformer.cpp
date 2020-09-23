
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force_s = 7.5f;
constexpr float player_move_force_max = 1.25f;
constexpr float player_jump_force = 3.0f;

// ----------------------------------------------------------------------------

bool ids_match( b2Contact* contact, unsigned id_1, unsigned id_2 )
{
	auto a_id = (unsigned) contact->GetFixtureA()->GetUserData().pointer;
	auto b_id = (unsigned) contact->GetFixtureB()->GetUserData().pointer;

	return( ( a_id == id_1 && b_id == id_2 ) || ( a_id == id_2 && b_id == id_1 ) );
}

void w_contact_listener::BeginContact( b2Contact* contact )
{
	if( ids_match( contact, IDF_PlayerFootSensor, IDF_World ) )
	{
		auto* layer = ( layer_platformer * )engine->layer_mgr->get_top();
		layer->player_on_ground++;
	}

	if( ids_match( contact, IDF_PlayerDropSensor, IDF_World ) )
	{
		auto* layer = (layer_platformer*) engine->layer_mgr->get_top();
		layer->player_drop_down_blocked++;
	}
}

void w_contact_listener::EndContact( b2Contact* contact )
{
	if( ids_match( contact, IDF_PlayerFootSensor, IDF_World ) )
	{
		auto* layer = (layer_platformer*) engine->layer_mgr->get_top();
		layer->player_on_ground--;
	}

	if( ids_match( contact, IDF_PlayerDropSensor, IDF_World ) )
	{
		auto* layer = (layer_platformer*) engine->layer_mgr->get_top();
		layer->player_drop_down_blocked--;
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
	timer_jump_limiter = std::make_unique<w_timer>( 100 );
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
			auto ypos = (float) y;
			float w = w_random::getf_range( 50, 200 );
			ec->add_fixture_line( IDF_World, w_vec2::zero, { xpos, ypos }, { xpos + w, ypos } );
		}

		for( int x = 0 ; x < 40 ; ++x )
		{
			float xpos = w_random::getf_range( 0.0f, v_window_w );
			float ypos = w_random::getf_range( 64.0f, v_window_h );

			if( w_random::geti_range( 0, 4 ) == 2 )
			{
				float sz = w_random::getf_range( 4, 16 );
				ec->add_fixture_circle( IDF_World, { xpos, ypos }, sz );
			}
			else
			{
				float sz = w_random::getf_range( 4, 32 );
				float sz2 = w_random::getf_range( 4, 32 );
				ec->add_fixture_box( IDF_World, w_rect( xpos, ypos, sz, sz2 ) );
			}
		}
	}

	// ----------------------------------------------------------------------------

	player = add_entity<e_platformer_player>();

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

	timer_jump_limiter->update();

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

	// query debugging

	//w_query_all q;
	//w_physics::point_check_all( player->pos + w_vec2( 0.0f, 12.0f ), clayer_world, &q );
	//fixtures_found_on_query = (int)q.fixtures.size();
}

void layer_platformer::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgb( w_color::teal );

	RENDER->draw_string( engine->pixel_font, fmt::format( "on ground   : {}", player_on_ground ), w_rect( 16, 16 ) );
	RENDER->draw_string( engine->pixel_font, fmt::format( "drop blocked: {}", player_drop_down_blocked ), w_rect( 16, 24 ) );

	//RENDER->draw_point( player->pos + w_vec2( 0.0f, 12.0f ) );

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
			if( player_on_ground && timer_jump_limiter->get_elapsed_count() )
			{
				timer_jump_limiter->reset();
				w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

				auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
				b2Vec2 current = ec->body->GetLinearVelocity();

				float dir_modifier = 1.0f;
				if( !fequals( left_stick.y, 0.0f ) && left_stick.y > 0.0f )
				{
					if( !player_drop_down_blocked )
					{
						dir_modifier = -0.5f;
						auto pos = ec->body->GetPosition();
						ec->body->SetTransform( { pos.x, pos.y + to_b2d( 12.0f ) }, 0.0f );
					}
					else
					{
						dir_modifier = 0.0f;
					}
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
