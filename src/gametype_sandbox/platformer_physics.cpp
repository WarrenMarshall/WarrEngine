
#include "app_header.h"

// ----------------------------------------------------------------------------

const float w_platformer_physics::player_move_force_s = 7.5f;
const float w_platformer_physics::player_move_force_max = 1.25f;
const float w_platformer_physics::player_jump_force = 3.0f;

// ----------------------------------------------------------------------------

w_platformer_physics::w_platformer_physics()
	: w_contact_listener()
{
	timer_jump_limiter = std::make_unique<w_timer>( 250 );
}

void w_platformer_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	if( contact_ids_match( contact_id::on_ground_sensor, contact_id::world ) )
	{
		player_on_ground++;
	}

	if( contact_ids_match( contact_id::can_drop_down_sensor, contact_id::world ) )
	{
		player_drop_down_blocked++;
	}

	if( contact_ids_match( contact_id::player, contact_id::coin ) )
	{
		auto coin = find_entity_from_contact_id( contact_id::coin );

		game->snd_plat_coin->play();
		coin->set_life_cycle( life_cycle::dying );
	}
}

void w_platformer_physics::EndContact( b2Contact* contact )
{
	w_contact_listener::EndContact( contact );

	if( contact_ids_match( contact_id::on_ground_sensor, contact_id::world ) )
	{
		player_on_ground--;
	}

	if( contact_ids_match( contact_id::can_drop_down_sensor, contact_id::world ) )
	{
		player_drop_down_blocked--;
	}
}

bool w_platformer_physics::can_jump()
{
	return (player_on_ground > 0);
}

bool w_platformer_physics::in_air()
{
	return ( player_on_ground == 0 );
}

bool w_platformer_physics::can_drop_down()
{
	return ( player_drop_down_blocked == 0 );
}

void w_platformer_physics::move_player( w_entity* player )
{
	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		if( in_air() )
		{
			left_stick.x *= player->air_control_damping;
		}

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( w_platformer_physics::player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = w_clamp( current.x, -w_platformer_physics::player_move_force_max, w_platformer_physics::player_move_force_max );

		ec->body->SetLinearVelocity( { desired, current.y } );
	}
}

void w_platformer_physics::jump_player( w_entity* player )
{
	if( can_jump() )
	{
		w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
		b2Vec2 current = ec->body->GetLinearVelocity();

		float dir_modifier = 1.0f;
		if( !fequals( left_stick.y, 0.0f ) && left_stick.y > 0.0f )
		{
			if( can_drop_down() )
			{
				dir_modifier = -0.25f;
				auto pos = ec->body->GetPosition();
				ec->body->SetTransform( { pos.x, pos.y + to_b2d( 12.0f ) }, 0.0f );
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

		timer_jump_limiter->reset();
		ec->body->SetLinearVelocity( { current.x, ( -w_platformer_physics::player_jump_force ) * dir_modifier } );
	}
}

void w_platformer_physics::update()
{
	timer_jump_limiter->update();
}
