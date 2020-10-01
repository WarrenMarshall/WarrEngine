
#include "app_header.h"

// ----------------------------------------------------------------------------

float w_twinstick_physics::player_move_force_s = 7.5f;
float w_twinstick_physics::player_base_radius = 8.0f;
float w_twinstick_physics::player_move_force_max = 0.75f;

// ----------------------------------------------------------------------------

w_twinstick_physics::w_twinstick_physics()
	: w_contact_listener()
{
}

void w_twinstick_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

#if 0
	if( contact_ids_match( contact_id::paddle, contact_id::ball ) )
	{
		e_twinstick_paddle* paddle = (e_twinstick_paddle*) find_entity_from_contact_id( contact_id::paddle );
		e_twinstick_ball* ball = (e_twinstick_ball*) find_entity_from_contact_id( contact_id::ball );

		ball->dir = ( ball->pos - paddle->pos ).normalize();
		ball->reset_velocity();

		game->snd_pong_ball_hit_paddle->play();
	}

	if( contact_ids_match( contact_id::ball, contact_id::ball ) )
	{
		e_twinstick_ball* ball_a = (e_twinstick_ball*) ( (w_entity_component*) contact->GetFixtureA()->GetBody()->GetUserData().pointer )->parent_entity;
		e_twinstick_ball* ball_b = (e_twinstick_ball*) ( (w_entity_component*) contact->GetFixtureB()->GetBody()->GetUserData().pointer )->parent_entity;

		b2Vec2 world_point = find_body_from_contact_id( contact_id::ball )->GetWorldPoint( manifold->localPoint );

		w_vec2 world_point_a = w_vec2( contact->GetFixtureA()->GetBody()->GetWorldPoint( manifold->localPoint ) ).from_b2d();
		w_vec2 world_point_b = w_vec2( contact->GetFixtureB()->GetBody()->GetWorldPoint( manifold->localPoint ) ).from_b2d();

		w_vec2 mid_point = world_point_a + ((world_point_b - world_point_a) * 0.5f);

		ball_a->dir = ( ball_a->pos - mid_point ).normalize();
		ball_b->dir = ( ball_b->pos - mid_point ).normalize();

		ball_a->reset_velocity();
		ball_b->reset_velocity();

		game->snd_pong_ball_hit_ball->play();
	}

	if( contact_ids_match( contact_id::ball, contact_id::world ) )
	{
		auto ball_body = find_body_from_contact_id( contact_id::ball );
		w_vec2 ball_dir = w_vec2( ball_body->GetLinearVelocity() );

		w_vec2 hit_normal = calc_hit_normal( ball_body );
		w_vec2 new_dir = w_vec2::reflect( ball_dir, hit_normal );
		fudge_movement_dir( new_dir );
		new_dir.normalize();

		e_twinstick_ball* ball = (e_twinstick_ball*) find_entity_from_contact_id( contact_id::ball );

		ball->dir = new_dir;
		ball->reset_velocity();

		game->snd_pong_ball_hit_world->play();
	}
#endif
}

void w_twinstick_physics::handle_user_input( w_entity* player )
{
	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x + left_stick.y, 0.0f ) )
	{
		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( w_twinstick_physics::player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		current.y += ( w_twinstick_physics::player_move_force_s * left_stick.y ) * w_time::FTS_step_value_s;
		w_vec2 desired = {
			w_clamp( current.x, -w_twinstick_physics::player_move_force_max, w_twinstick_physics::player_move_force_max ),
			w_clamp( current.y, -w_twinstick_physics::player_move_force_max, w_twinstick_physics::player_move_force_max )
		};

		ec->body->SetLinearVelocity( { desired.x, desired.y } );
	}

	w_vec2 right_stick = engine->input->get_axis_state( input_id::controller_right_stick );

	if( !fequals( right_stick.x + right_stick.y, 0.0f ) )
	{
		float angle = w_vec2::to_angle( w_vec2( right_stick.normalize() ) );
		player->set_angle_deep( angle );
	}
}

