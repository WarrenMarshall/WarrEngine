
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force = 25.0f;
constexpr float player_move_force_max = 100.0f;
constexpr float player_jump_force = 325.0f;

constexpr float player_base_radius = 6.0f;
constexpr float player_drop_down_normal_tolerance = 0.8f;
constexpr int player_jump_interval = 50;
constexpr float player_air_control_damping = 0.5f;

// ----------------------------------------------------------------------------

w_platformer_physics::w_platformer_physics()
	: w_contact_listener()
{
	timer_jump_limiter = std::make_unique<w_timer>( player_jump_interval );
}

void w_platformer_physics::BeginContact( b2Contact* contact )
{
	w_contact_listener::BeginContact( contact );

	if( contact_ids_match( "s_on_ground", "world" ) )
	{
		player_on_ground++;

		if( !in_air() )
		{
			hit_ground();
		}
	}
	else if( contact_ids_match( "s_can_drop_down", "world" ) )
	{
		player_drop_down_blocked++;
	}
	else if( contact_ids_match( "player", "coin" ) )
	{
		auto coin = find_entity_from_contact_id( "coin" );

		game->snd_coin->play();
		coin->set_life_cycle( life_cycle::dying );
	}
}

void w_platformer_physics::EndContact( b2Contact* contact )
{
	w_contact_listener::EndContact( contact );

	if( contact_ids_match( "s_on_ground", "world" ) )
	{
		player_on_ground--;
	}
	else if( contact_ids_match( "s_can_drop_down", "world" ) )
	{
		player_drop_down_blocked--;
	}
}

bool w_platformer_physics::can_jump() const
{
	return ( player_on_ground > 0 && timer_jump_limiter->get_elapsed_count() );
}

bool w_platformer_physics::in_air() const
{
	return ( player_on_ground == 0 );
}

bool w_platformer_physics::on_ground() const
{
	return ( player_on_ground > 0 );
}

bool w_platformer_physics::can_drop_down() const
{
	return ( player_drop_down_blocked == 0 );
}

void w_platformer_physics::hit_ground()
{
	auto player = LAYER->find_entity_from_tag( "player" );

	if( player->phys_get_primary_body()->body->GetLinearVelocity().y > 0.0f )
	{
		timer_jump_limiter->reset();

		w_raycast_closest hit;
		if( trace_closest( player->pos, w_vec2::down, 16.0f, clayer_world, &hit ) )
		{
			auto fx = LAYER->add_entity<w_entity_fx>();
			fx->set_position( hit.result.pos + w_vec2(0,2) );
			fx->add_component<ec_emitter>()->init( "small_smoke_puff" );
		}
	}
}

void w_platformer_physics::handle_user_input()
{
	auto player = LAYER->find_entity_from_tag( "player" );
	w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

	if( !fequals( left_stick.x, 0.0f ) )
	{
		player->get_component<ec_sprite>( component_type::sprite )->flip_x = ( left_stick.x < 0.0f );

		if( in_air() )
		{
			left_stick.x *= player_air_control_damping;
		}

		auto ec = player->get_component<ec_b2d_body>( component_type::b2d_dynamic | component_type::b2d_kinematic );

		w_vec2 current_velocity = w_vec2( ec->body->GetLinearVelocity() ).from_b2d();
		float force_to_be_applied = 0.0f;

		if( std::fabs( current_velocity.x ) < player_move_force_max )
		{
			force_to_be_applied = player_move_force * left_stick.x;

			float force_final = ec->body->GetMass() * force_to_be_applied / (1.0f / w_time::FTS_desired_frames_per_second);
			ec->body->ApplyForceToCenter( w_vec2( to_b2d( force_final ), 0.0f ).as_b2Vec2(), true );
		}
	}
}

bool w_platformer_physics::handle_input_event( const w_input_event* evt )
{
	auto player = LAYER->find_entity_from_tag( "player" );

	if( evt->event_id == event_id::input_pressed )
	{
		if( evt->input_id == input_id::controller_button_a )
		{
			if( can_jump() )
			{
				timer_jump_limiter->reset();

				w_vec2 left_stick = engine->input->get_axis_state( input_id::controller_left_stick );

				auto ec = player->phys_get_primary_body();

				float dir_modifier = 1.0f;
				if( left_stick.y > player_drop_down_normal_tolerance )
				{
					if( can_drop_down() )
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
					game->snd_jump->play();
				}

				if( dir_modifier < 0.0f )
				{
					game->snd_drop_down->play();
				}

				float force_final = ec->body->GetMass() * (-player_jump_force * dir_modifier) / ( 1.0f / w_time::FTS_desired_frames_per_second );
				ec->body->ApplyForceToCenter( w_vec2( 0.0f, to_b2d( force_final ) ).as_b2Vec2(), true );

				return true;
			}
		}
	}

	return false;
}

void w_platformer_physics::update()
{
	timer_jump_limiter->update();

	auto player = LAYER->find_entity_from_tag( "player" );

	if( !player )
	{
		return;
	}

	auto ec = player->get_component<ec_sprite>( component_type::sprite );
	ec->tex = a_anim_texture::find( "anim_player_idle" );

	auto ec_b2d = player->phys_get_primary_body();
	w_vec2 vel = w_vec2( ec_b2d->body->GetLinearVelocity() ).from_b2d();

	if( !on_ground() )
	{
		ec->tex = a_anim_texture::find( "anim_player_jump" );
	}
	else
	{
		// #todo - vary animation speed based on running speed
		if( !fequals( vel.x, 0.0f ) )
		{
			ec->tex = a_anim_texture::find( "anim_player_run" );
		}
	}
}
