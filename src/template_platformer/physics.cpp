
#include "app_header.h"

// ----------------------------------------------------------------------------

w_platformer_physic_responder::w_platformer_physic_responder()
{
	timer_jump_limiter = std::make_unique<w_timer>( player_jump_interval );
}

void w_platformer_physic_responder::BeginContact( b2Contact* contact )
{
	w_physics_responder::BeginContact( contact );

	if( contact_tags_match( "s_on_ground", "world" ) )
	{
		player_on_ground++;

		if( !in_air() )
		{
			hit_ground();
		}
	}
	else if( contact_tags_match( "s_can_drop_down", "world" ) )
	{
		player_drop_down_blocked++;
	}
	else if( contact_tags_match( "player", "coin" ) )
	{
		auto coin = find_entity_from_contact_tag( "coin" );

		game->snd_coin->play();
		coin->ilc_set( life_cycle::dying );
	}
}

void w_platformer_physic_responder::EndContact( b2Contact* contact )
{
	w_physics_responder::EndContact( contact );

	if( contact_tags_match( "s_on_ground", "world" ) )
	{
		player_on_ground--;
	}
	else if( contact_tags_match( "s_can_drop_down", "world" ) )
	{
		player_drop_down_blocked--;
	}
}

bool w_platformer_physic_responder::can_jump() const
{
	return ( player_on_ground > 0 && timer_jump_limiter->get_elapsed_count() );
}

bool w_platformer_physic_responder::in_air() const
{
	return ( player_on_ground == 0 );
}

bool w_platformer_physic_responder::on_ground() const
{
	return ( player_on_ground > 0 );
}

bool w_platformer_physic_responder::can_drop_down() const
{
	return ( player_drop_down_blocked == 0 );
}

void w_platformer_physic_responder::hit_ground()
{
	auto player = LAYER->find_entity_from_tag( "player" );

	if( player->phys_get_primary_body()->body->GetLinearVelocity().y > 0.0f )
	{
		timer_jump_limiter->reset();

		w_raycast_closest hit;
		if( w_physics_query::trace_closest( player->pos, w_vec2::down, 16.0f, clayer_world, &hit ) )
		{
			auto fx = LAYER->add_entity<w_entity_transient>();
			fx->it_set_position( hit.result.pos + w_vec2(0,2) );
			fx->add_component<ec_emitter>()->init( "small_smoke_puff" );
		}
	}
}

void w_platformer_physic_responder::handle_user_input()
{
}

void w_platformer_physic_responder::update()
{
	timer_jump_limiter->update();
}
