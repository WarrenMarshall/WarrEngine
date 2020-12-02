
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force = 25.0f;
constexpr float player_move_force_max = 80.0f;
constexpr float player_jump_force = 300.0f;

constexpr float player_base_radius = 6.0f;
constexpr float player_drop_down_normal_tolerance = 0.8f;
constexpr int player_jump_interval = 50;
constexpr float player_air_control_damping = 1.0f;

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
