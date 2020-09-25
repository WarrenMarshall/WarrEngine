
#include "app_header.h"

// ----------------------------------------------------------------------------

const float platformer_physics::player_move_force_s = 7.5f;
const float platformer_physics::player_move_force_max = 1.25f;
const float platformer_physics::player_jump_force = 3.0f;

// ----------------------------------------------------------------------------

platformer_physics::platformer_physics()
	: w_contact_listener()
{
	timer_jump_limiter = std::make_unique<w_timer>( 100 );
}

void platformer_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	timer_jump_limiter->update();

	if( sensor_ids_match( { sensor_id::ground, sensor_id::world } ) )
	{
		player_on_ground++;
	}

	if( sensor_ids_match( { sensor_id::area_01, sensor_id::world } ) )
	{
		player_drop_down_blocked++;
	}
}

void platformer_physics::EndContact( b2Contact* contact )
{
	w_contact_listener::EndContact( contact );

	if( sensor_ids_match( { sensor_id::ground, sensor_id::world } ) )
	{
		player_on_ground--;
	}

	if( sensor_ids_match( { sensor_id::area_01, sensor_id::world } ) )
	{
		player_drop_down_blocked--;
	}
}

bool platformer_physics::can_jump()
{
	return (player_on_ground > 0);
}

bool platformer_physics::in_air()
{
	return ( player_on_ground == 0 );
}

bool platformer_physics::can_drop_down()
{
	return ( player_drop_down_blocked == 0 );
}

void platformer_physics::move_player( w_entity* player )
{
	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		if( game->plat_physics->in_air() )
		{
			left_stick.x *= player->air_control_damping;
		}

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( platformer_physics::player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = w_clamp( current.x, -platformer_physics::player_move_force_max, platformer_physics::player_move_force_max );

		ec->body->SetLinearVelocity( { desired, current.y } );
	}
}

void platformer_physics::jump_player( w_entity* player )
{
	if( can_jump() )
	{
		w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic );
		b2Vec2 current = ec->body->GetLinearVelocity();

		float dir_modifier = 1.0f;
		if( !fequals( left_stick.y, 0.0f ) && left_stick.y > 0.0f )
		{
			if( game->plat_physics->can_drop_down() )
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

		game->plat_physics->timer_jump_limiter->reset();
		ec->body->SetLinearVelocity( { current.x, ( -platformer_physics::player_jump_force ) * dir_modifier } );
	}
}