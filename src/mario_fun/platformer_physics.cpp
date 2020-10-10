
#include "app_header.h"

// ----------------------------------------------------------------------------

float w_platformer_physics::player_move_force_s = 7.5f;
float w_platformer_physics::player_base_radius = 8.0f;
float w_platformer_physics::player_move_force_max = 1.25f;
float w_platformer_physics::player_jump_force = 3.0f;
float w_platformer_physics::player_drop_down_normal_tolerance = 0.8f;
int w_platformer_physics::player_jump_interval = 150;
float w_platformer_physics::player_air_control_damping = 0.35f;

// ----------------------------------------------------------------------------

w_platformer_physics::w_platformer_physics()
	: w_contact_listener()
{
	timer_jump_limiter = std::make_unique<w_timer>( w_platformer_physics::player_jump_interval );
}

void w_platformer_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	if( contact_ids_match( "s_on_ground", "world" ) )
	{
		player_on_ground++;
		if( !in_air() )
		{
			timer_jump_limiter->reset();
		}
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

void w_platformer_physics::EndContact( b2Contact* contact )
{
	w_contact_listener::EndContact( contact );

	if( contact_ids_match( "s_on_ground", "world" ) )
	{
		player_on_ground--;
	}

	if( contact_ids_match( "s_can_drop_down", "world" ) )
	{
		player_drop_down_blocked--;
	}
}

bool w_platformer_physics::can_jump()
{
	return ( player_on_ground > 0 && timer_jump_limiter->get_elapsed_count() );
}

bool w_platformer_physics::in_air()
{
	return ( player_on_ground == 0 );
}

bool w_platformer_physics::can_drop_down()
{
	return ( player_drop_down_blocked == 0 );
}

void w_platformer_physics::handle_user_input( w_entity* player )
{
	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		player->get_component<ec_sprite>( component_type::sprite )->flip_x = ( left_stick.x < 0.0f );

		if( in_air() )
		{
			left_stick.x *= w_platformer_physics::player_air_control_damping;
		}

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( w_platformer_physics::player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = std::clamp( current.x, -w_platformer_physics::player_move_force_max, w_platformer_physics::player_move_force_max );

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
			if( left_stick.y > w_platformer_physics::player_drop_down_normal_tolerance )
			{
				if( can_drop_down() )
				{
					dir_modifier = -0.25f;
					auto pos = ec->body->GetPosition();
					ec->body->SetTransform( { pos.x, pos.y + to_b2d( w_platformer_physics::player_base_radius * 1.5f ) }, 0.0f );
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
			ec->body->SetLinearVelocity( { current.x, ( -w_platformer_physics::player_jump_force ) * dir_modifier } );
		}
	}
}

void w_platformer_physics::update()
{
	timer_jump_limiter->update();

	//auto ec = player->get_component<ec_sprite>( component_type::sprite );
	//if( in_air)
	//{
	//	ec->
	//}
}
