
#include "app_header.h"

// ----------------------------------------------------------------------------

bool w_player_input_receiver::iir_on_motion( const w_input_event* evt )
{
	// movement left/right

	if( evt->input_id == input_id::gamepad_left_stick )
	{
		auto phys_responder = (w_platformer_physic_responder*) game->physics_responder.get();
		auto player = LAYER->find_entity_from_tag( ( "player" ) );

		w_vec2 left_stick = engine->input->get_axis_state( input_id::gamepad_left_stick );

		if( !fequals( left_stick.x, 0.0f ) )
		{
			player->get_component<ec_sprite>( component_type::sprite )->flip_x = ( left_stick.x < 0.0f );

			if( phys_responder->in_air() )
			{
				left_stick.x *= player_air_control_damping;
			}

			auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

			w_vec2 current_velocity = w_vec2( ec->body->GetLinearVelocity() ).from_b2d();

			if( std::fabs( current_velocity.x ) < player_move_force_max )
			{
				float force_to_be_applied =
					std::min(
						player_move_force_max,
						ec->body->GetMass() * ( player_move_force * left_stick.x )
					)
					* w_time::FTS_desired_frames_per_second;
				ec->body->ApplyForceToCenter( w_vec2( to_b2d( force_to_be_applied ), 0.0f ).as_b2Vec2(), true );
			}
		}

		return true;
	}

	return false;
}

bool w_player_input_receiver::iir_on_pressed( const w_input_event* evt )
{
	// jump

	if( evt->input_id == input_id::gamepad_button_a )
	{
		auto phys_responder = (w_platformer_physic_responder*) game->physics_responder.get();

		if( phys_responder->can_jump() )
		{
			auto player = LAYER->find_entity_from_tag( ( "player" ) );

			phys_responder->timer_jump_limiter->reset();

			w_vec2 left_stick = engine->input->get_axis_state( input_id::gamepad_left_stick );

			auto ec = player->phys_get_primary_body();

			float dir_modifier = 1.0f;
			if( left_stick.y > player_drop_down_normal_tolerance )
			{
				if( phys_responder->can_drop_down() )
				{
					dir_modifier = -0.25f;
					auto pos = ec->body->GetPosition();
					ec->body->SetTransform( { pos.x, pos.y + to_b2d( player_base_radius * 1.5f ) }, 0.0f );
				}
				else
				{
					dir_modifier = 0.0f;
				}
			}

			if( dir_modifier > 0.0f )
			{
				//game->snd_jump->play();

			}

			if( dir_modifier < 0.0f )
			{
				//game->snd_drop_down->play();
			}

			float force_to_be_applied = ec->body->GetMass() * ( -player_jump_force * dir_modifier );
			ec->body->ApplyLinearImpulseToCenter( w_vec2( 0.0f, to_b2d( force_to_be_applied ) ).as_b2Vec2(), true );

			return true;
		}
	}

	return false;
}
