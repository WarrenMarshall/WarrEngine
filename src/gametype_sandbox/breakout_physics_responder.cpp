
#include "app_header.h"

// ----------------------------------------------------------------------------

w_breakout_physics_responder::w_breakout_physics_responder()
	: w_physics_responder()
{
}

void w_breakout_physics_responder::BeginContact( b2Contact* contact )
{
	w_physics_responder::BeginContact( contact );

	if( contact_ids_match( "paddle", "ball" ) )
	{
		auto paddle = (e_breakout_paddle*) find_entity_from_contact_id( "paddle" );
		auto ball = (e_breakout_ball*) find_entity_from_contact_id( "ball" );

		ball->dir = ( ball->pos - paddle->pos ).normalize();
		ball->reset_velocity();

		game->snd_pong_ball_hit_paddle->play();
	}

	if( contact_ids_match( "ball", "ball" ) )
	{
		auto ball_a = (e_breakout_ball*) ( (w_entity_component*) contact->GetFixtureA()->GetBody()->GetUserData().pointer )->parent_entity;
		auto ball_b = (e_breakout_ball*) ( (w_entity_component*) contact->GetFixtureB()->GetBody()->GetUserData().pointer )->parent_entity;

		b2Vec2 world_point = find_body_from_contact_id( "ball" )->GetWorldPoint( manifold->localPoint );

		w_vec2 world_point_a = w_vec2( contact->GetFixtureA()->GetBody()->GetWorldPoint( manifold->localPoint ) ).from_b2d();
		w_vec2 world_point_b = w_vec2( contact->GetFixtureB()->GetBody()->GetWorldPoint( manifold->localPoint ) ).from_b2d();

		w_vec2 mid_point = world_point_a + ((world_point_b - world_point_a) * 0.5f);

		ball_a->dir = ( ball_a->pos - mid_point ).normalize();
		ball_b->dir = ( ball_b->pos - mid_point ).normalize();

		ball_a->reset_velocity();
		ball_b->reset_velocity();

		game->snd_pong_ball_hit_ball->play();
	}

	if( contact_ids_match( "ball", "world" ) )
	{
		auto ball_body = find_body_from_contact_id( "ball" );
		auto ball_dir = w_vec2( ball_body->GetLinearVelocity() );

		w_vec2 hit_normal = calc_hit_normal( ball_body );
		w_vec2 new_dir = w_vec2::reflect_across_normal( ball_dir, hit_normal );

		// fudges a value so it's at least 0.3 in it's value

		auto fudge_value = [] ( float& val )
		{
			if( val < 0.0f )
			{
				val = std::min( -0.3f, val );
			}
			else
			{
				val = std::max( 0.3f, val );
			}
		};

		fudge_value( new_dir.x );
		fudge_value( new_dir.y );

		new_dir.normalize();

		e_breakout_ball* ball = (e_breakout_ball*) find_entity_from_contact_id( "ball" );

		ball->dir = new_dir;
		ball->reset_velocity();

		game->snd_pong_ball_hit_world->play();
	}
}
