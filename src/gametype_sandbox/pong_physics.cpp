
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

	if( dir.x < 0.0f )
	{
		dir.x = w_min( -0.3f, dir.x );
	}
	else
	{
		dir.x = w_max( 0.3f, dir.x );
	}
}

// ----------------------------------------------------------------------------


w_pong_physics::w_pong_physics()
	: w_contact_listener()
{
}

void w_pong_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	if( contact_ids_match( { contact_id::paddle, contact_id::ball } ) )
	{
		e_pong_paddle* paddle = (e_pong_paddle*) find_entity_from_contact_id( contact_id::paddle );
		e_pong_ball* ball = (e_pong_ball*) find_entity_from_contact_id( contact_id::ball );

		ball->dir = ( ball->pos - paddle->pos ).normalize();
		ball->reset_velocity();
	}

	if( contact_ids_match( { contact_id::ball, contact_id::ball } ) )
	{
		e_pong_ball* ball_a = (e_pong_ball*) ( (w_entity_component*) contact->GetFixtureA()->GetBody()->GetUserData().pointer )->parent_entity;
		e_pong_ball* ball_b = (e_pong_ball*) ( (w_entity_component*) contact->GetFixtureB()->GetBody()->GetUserData().pointer )->parent_entity;

		w_vec2 save = ball_a->dir;
		ball_a->dir = ball_b->dir * -1.0f;
		ball_b->dir = save * -1.0f;

		ball_a->reset_velocity();
		ball_b->reset_velocity();

		game->snd_pong_ball_hit_ball->play();
	}

	if( contact_ids_match( { contact_id::ball, contact_id::world } ) )
	{
		auto ball_body = find_body_from_contact_id( contact_id::ball );
		w_vec2 ball_dir = w_vec2( ball_body->GetLinearVelocity() );

		w_vec2 hit_normal = calc_hit_normal( ball_body );
		w_vec2 new_dir = w_vec2::reflect( ball_dir, hit_normal );
		fudge_movement_dir( new_dir );
		new_dir.normalize();

		e_pong_ball* ball = (e_pong_ball*) find_entity_from_contact_id( contact_id::ball );

		ball->dir = new_dir;
		ball->reset_velocity();

		game->snd_pong_ball_hit_world->play();
	}
}
