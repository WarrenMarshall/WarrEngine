
#include "app_header.h"

// ----------------------------------------------------------------------------

layer_platformer::layer_platformer()
{
	draws_completely_solid = true;
}

void layer_platformer::push()
{
	engine->box2d_world->SetContactListener( my_game->plat_physics.get() );

	ec_b2d_body* ec = nullptr;

	world_geo = add_entity<w_entity>();
	world_geo->collision_layer = clayer_world;
	world_geo->collides_with = clayer_player;
	world_geo->draw_debug_info = true;
	ec = world_geo->add_component<ec_b2d_static>();
	{
		// bounding box for world

		ec->add_fixture_line_loop(
			sensor_id::world,
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
			ec->add_fixture_line( sensor_id::world, w_vec2::zero, { xpos, ypos }, { xpos + w, ypos } );
		}

		for( int x = 0 ; x < 25 ; ++x )
		{
			float xpos = w_random::getf_range( 0.0f, v_window_w );
			float ypos = w_random::getf_range( 64.0f, v_window_h );

			if( w_random::geti_range( 0, 4 ) == 2 )
			{
				float sz = w_random::getf_range( 4, 16 );
				ec->add_fixture_circle( sensor_id::world, { xpos, ypos }, sz );
			}
			else
			{
				float sz = w_random::getf_range( 4, 32 );
				float sz2 = w_random::getf_range( 4, 32 );
				ec->add_fixture_box( sensor_id::world, w_rect( xpos, ypos, sz, sz2 ) );
			}
		}
	}

	// ----------------------------------------------------------------------------

	player = add_entity<e_platformer_player>();
}

void layer_platformer::update()
{
	w_layer::update();

	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		if( my_game->plat_physics->in_air() )
		{
			left_stick.x *= 0.5f;
		}

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( platformer_physics::player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = w_clamp( current.x, -platformer_physics::player_move_force_max, platformer_physics::player_move_force_max );

		ec->body->SetLinearVelocity( { desired, current.y } );
	}
}

void layer_platformer::draw()
{
	w_layer::draw();

	RENDER
		->begin()
		->push_rgb( w_color::teal );

	RENDER->draw_string( engine->pixel_font, fmt::format( "on ground   : {}", !my_game->plat_physics->in_air() ), w_rect( 16, 16 ) );
	RENDER->draw_string( engine->pixel_font, fmt::format( "drop blocked: {}", !my_game->plat_physics->can_drop_down() ), w_rect( 16, 24 ) );

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
	}

	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::key_space || evt->input_id == input_id::controller_button_a )
		{
			if( my_game->plat_physics->can_jump() )
			{
				w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

				auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
				b2Vec2 current = ec->body->GetLinearVelocity();

				float dir_modifier = 1.0f;
				if( !fequals( left_stick.y, 0.0f ) && left_stick.y > 0.0f )
				{
					if( my_game->plat_physics->can_drop_down() )
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

				my_game->plat_physics->timer_jump_limiter->reset();
				ec->body->SetLinearVelocity( { current.x, (-platformer_physics::player_jump_force) * dir_modifier } );
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
