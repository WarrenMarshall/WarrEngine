
#include "app_header.h"

// ----------------------------------------------------------------------------

float w_twinstick_contact_listener::player_move_force_s = 15.5f;
float w_twinstick_contact_listener::player_base_radius = 8.0f;
float w_twinstick_contact_listener::player_move_force_max = 0.75f;

// ----------------------------------------------------------------------------

w_twinstick_contact_listener::w_twinstick_contact_listener()
	: w_physics_responder()
{
}

void w_twinstick_contact_listener::BeginContact( b2Contact* contact )
{
	w_physics_responder::BeginContact( contact );

}

void w_twinstick_contact_listener::handle_user_input( w_entity* player )
{
	//auto player = LAYER->find_entity_from_tag( "player" );

	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x + left_stick.y, 0.0f ) )
	{
		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( w_twinstick_contact_listener::player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		current.y += ( w_twinstick_contact_listener::player_move_force_s * left_stick.y ) * w_time::FTS_step_value_s;
		w_vec2 desired = {
			std::clamp( current.x, -w_twinstick_contact_listener::player_move_force_max, w_twinstick_contact_listener::player_move_force_max ),
			std::clamp( current.y, -w_twinstick_contact_listener::player_move_force_max, w_twinstick_contact_listener::player_move_force_max )
		};

		ec->body->SetLinearVelocity( { desired.x, desired.y } );
	}

	w_vec2 right_stick = engine->input->get_axis_state( input_id::controller_right_stick, b_ignore_dead_zone( true ) );

	if( !fequals( right_stick.x + right_stick.y, 0.0f ) )
	{
		float angle = w_vec2::angle_from_dir( w_vec2( right_stick.normalize() ) );
		player->set_angle_deep( angle );
	}
}

#if 0
void w_twinstick_physics::update()
{
	trace_hit_location = w_vec2::zero;

	auto player = LAYER->find_entity_from_tag( "player" );
	if( player )
	{
		w_raycast_closest hit;
		if( w_physics_checks::trace_closest( player->pos, w_vec2::dir_from_angle( player->angle ), 500, clayer_world, &hit ) )
		{
			trace_hit_location = hit.result.pos;
		}
	}
}
#endif
