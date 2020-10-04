
#include "app_header.h"

// ----------------------------------------------------------------------------

void fudge_movement_dir( w_vec2& dir )
{
	if( dir.y < 0.0f )
	{
		dir.y = w_min( -0.3f, dir.y );
	}
	else
	{
		dir.y = w_max( 0.3f, dir.y );
	}

#if 1
	// this was added when I did the new code but I THINK it causes problems sometimes
	// ... so delete it eventually if you never use it
	if( dir.x < 0.0f )
	{
		dir.x = w_min( -0.3f, dir.x );
	}
	else
	{
		dir.x = w_max( 0.3f, dir.x );
	}
#endif
}

// ----------------------------------------------------------------------------


w_breakout_physics::w_breakout_physics()
	: w_contact_listener()
{
}

void w_breakout_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	if( contact_ids_match( "paddle", "ball" ) )
	{
		e_breakout_paddle* paddle = (e_breakout_paddle*) find_entity_from_contact_id( "paddle" );
		e_breakout_ball* ball = (e_breakout_ball*) find_entity_from_contact_id( "ball" );

		ball->dir = ( ball->pos - paddle->pos ).normalize();
		ball->reset_velocity();

		game->snd_pong_ball_hit_paddle->play();
	}

	if( contact_ids_match( "ball", "ball" ) )
	{
		e_breakout_ball* ball_a = (e_breakout_ball*) ( (w_entity_component*) contact->GetFixtureA()->GetBody()->GetUserData().pointer )->parent_entity;
		e_breakout_ball* ball_b = (e_breakout_ball*) ( (w_entity_component*) contact->GetFixtureB()->GetBody()->GetUserData().pointer )->parent_entity;

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
		w_vec2 ball_dir = w_vec2( ball_body->GetLinearVelocity() );

		w_vec2 hit_normal = calc_hit_normal( ball_body );
		w_vec2 new_dir = w_vec2::reflect( ball_dir, hit_normal );
		fudge_movement_dir( new_dir );
		new_dir.normalize();

		e_breakout_ball* ball = (e_breakout_ball*) find_entity_from_contact_id( "ball" );

		ball->dir = new_dir;
		ball->reset_velocity();

		game->snd_pong_ball_hit_world->play();
	}
}
