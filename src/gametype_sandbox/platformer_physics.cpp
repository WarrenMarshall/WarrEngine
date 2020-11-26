
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force = 25.0f;
constexpr float player_move_force_max = 80.0f;
constexpr float player_jump_force = 300.0f;

constexpr float player_base_radius = 6.0f;
constexpr float player_drop_down_normal_tolerance = 0.8f;
constexpr int player_jump_interval = 50;
constexpr float player_air_control_damping = 0.35f;

// ----------------------------------------------------------------------------

w_platformer_contact_listener::w_platformer_contact_listener()
	: w_physics_responder()
{
	timer_jump_limiter = std::make_unique<w_timer>( player_jump_interval );
}

void w_platformer_contact_listener::BeginContact( b2Contact* contact )
{
	w_physics_responder::BeginContact( contact );

	if( contact_ids_match( "s_on_ground", "world" ) )
	{
		player_on_ground++;
	}

	if( contact_ids_match( "s_can_drop_down", "world" ) )
	{
		player_drop_down_blocked++;
	}

	if( contact_ids_match( "player", "coin" ) )
	{
		auto coin = find_entity_from_contact_id( "coin" );

		game->snd_plat_coin->play();
		coin->set_life_cycle( life_cycle::dying );
	}
}

void w_platformer_contact_listener::EndContact( b2Contact* contact )
{
	w_physics_responder::EndContact( contact );

	if( contact_ids_match( "s_on_ground", "world" ) )
	{
		player_on_ground--;
	}

	if( contact_ids_match( "s_can_drop_down", "world" ) )
	{
		player_drop_down_blocked--;
	}
}

bool w_platformer_contact_listener::can_jump()
{
	return ( player_on_ground > 0 && timer_jump_limiter->get_elapsed_count() );
}

bool w_platformer_contact_listener::in_air()
{
	return ( player_on_ground == 0 );
}

bool w_platformer_contact_listener::can_drop_down()
{
	return ( player_drop_down_blocked == 0 );
}

#if 0
void w_platformer_physics::event_input_motion( w_entity* player )
{
	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		if( in_air() )
		{
			left_stick.x *= player_air_control_damping;
		}

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( player_move_force * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = std::clamp( current.x, -player_move_force_max, player_move_force_max );

		ec->body->SetLinearVelocity( { desired, current.y } );
	}

	if( engine->input->get_button_state( input_id::controller_button_a ) == button_state::held )
	{
		if( can_jump() )
		{
			timer_jump_limiter->reset();

			w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

			auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

			float dir_modifier = 1.0f;
			if( left_stick.y > player_drop_down_normal_tolerance )
			{
				if( can_drop_down() )
				{
					dir_modifier = -0.25f;
					auto pos = ec->body->GetPosition();
					ec->body->SetTransform( { pos.x, pos.y + to_b2d( player_base_radius * 1.5f ) }, 0.0f );
				}
				else
				{
					dir_modifier = 0.0f;
				}
			}

			if( dir_modifier > 0.0f )
			{
				game->snd_plat_jump->play();
			}

			if( dir_modifier < 0.0f )
			{
				game->snd_plat_drop_down->play();
			}

			b2Vec2 current = ec->body->GetLinearVelocity();
			ec->body->SetLinearVelocity( { current.x, ( -player_jump_force ) * dir_modifier } );
		}
	}
}

void w_platformer_physics::update()
{
	timer_jump_limiter->update();
}
#endif
