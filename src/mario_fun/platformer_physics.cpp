
#include "app_header.h"

// ----------------------------------------------------------------------------

constexpr float player_move_force_s = 7.5f;
constexpr float player_base_radius = 6.0f;
constexpr float player_move_force_max = 1.0f;

constexpr float player_jump_force = 6.5f;
constexpr float player_drop_down_normal_tolerance = 0.8f;
constexpr int player_jump_interval = 200;
constexpr float player_air_control_damping = 0.35f;

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

void w_platformer_physics::EndContact( b2Contact* contact )
{
	w_contact_listener::EndContact( contact );

	if( contact_ids_match( "s_on_ground", "world" ) )
	{
		player_on_ground--;
	}

	if( contact_ids_match( "s_can_drop_down", "world" ) )
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
	if( player->get_primary_body()->body->GetLinearVelocity().y > 0.0f )
	{
		timer_jump_limiter->reset();

		w_raycast_closest hit;
		if( trace_closest( player->pos, w_vec2::down, 16.0f, clayer_world, &hit ) )
		{
			auto fx = LAYER->add_entity<w_entity_fx>();
			fx->set_position( hit.result.point + w_vec2(0,2) );
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
		b2Vec2 current = ec->body->GetLinearVelocity();
		current.x += ( player_move_force_s * left_stick.x ) * w_time::FTS_step_value_s;
		float desired = std::clamp( current.x, -player_move_force_max, player_move_force_max );

		ec->body->SetLinearVelocity( { desired, current.y } );
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

				auto ec = player->get_primary_body();

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
					game->snd_plat_jump->play();
				}

				if( dir_modifier < 0.0f )
				{
					game->snd_plat_drop_down->play();
				}

				ec->body->ApplyLinearImpulseToCenter( w_vec2( 0.0f, -player_jump_force * dir_modifier ).to_b2d(), true );

				return true;
			}
		}
	}

	return false;
}

void w_platformer_physics::update()
{
	timer_jump_limiter->update();

	auto e = LAYER->find_entity_from_tag( "player" );

	if( !e )
	{
		return;
	}

	auto ec = e->get_component<ec_sprite>( component_type::sprite );
	ec->tex = engine->get_asset<a_anim_texture>( "anim_player_idle" );

	auto ec_b2d = e->get_primary_body();
	b2Vec2 vel = ec_b2d->body->GetLinearVelocity();

	if( !on_ground() )
	{
		ec->tex = engine->get_asset<a_anim_texture>( "anim_player_jump" );
	}
	else
	{
		// #todo - vary animation speed based on running speed
		if( !fequals( vel.x, 0.0f ) )
		{
			ec->tex = engine->get_asset<a_anim_texture>( "anim_player_run" );
		}
	}
}
