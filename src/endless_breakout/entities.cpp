
#include "app_header.h"

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
}

// ----------------------------------------------------------------------------

e_paddle::e_paddle()
{
	add_component<ec_sprite>()->init( "sub_tex_paddle" );
	add_component<ec_collider>()->init_as_box( w_rect( -48, -8, 96, 16 ) );

	collision_layer = cl_paddle;
	collides_with = 0;
}

// ----------------------------------------------------------------------------

powerup::powerup()
{
	timer = std::make_unique<w_timer>( 100 );
}

void powerup::activate( w_entity* owner )
{
	timer->interval_ms = 15000 ;
	timer->reset();

	is_active = true;
}

void powerup::deactivate( w_entity* owner )
{
	is_active = false;
}

void powerup::update()
{
	timer->update();
}

// ----------------------------------------------------------------------------

pu_fireball::pu_fireball()
	: powerup()
{
	snd_fire_on = engine->get_asset<a_sound>( "fire_on" );
	snd_fire_off = engine->get_asset<a_sound>( "fire_off" );
}

void pu_fireball::activate( w_entity* owner )
{
	snd_fire_on->play();

	if( !is_active )
	{
		{
			auto ec = owner->add_component<ec_emitter>();
			ec->init( "fireball_trail" );
			clean_up.push_back( ec );
		}
	}

	powerup::activate( owner );

	timer->reset();
}

void pu_fireball::deactivate( w_entity* owner )
{
	snd_fire_off->play();

	powerup::deactivate( owner );

	for( auto ec : clean_up )
	{
		ec->set_life_cycle( life_cycle::dying );
	}
	clean_up.clear();
}

// ----------------------------------------------------------------------------

e_ball::e_ball()
{
	add_component<ec_sprite>()->init( "sub_tex_ball" );
	add_component<ec_collider>()->init_as_circle( 4 );
	add_component<ec_force_constant>()->init( w_random::getf_range( 0, 359 ), 150.0f );

	collision_layer = cl_ball;
	collides_with = cl_ball | cl_wall | cl_brick | cl_paddle | cl_deathzone | cl_powup_multiball | cl_powup_fireball;
}

void e_ball::update()
{
	w_entity::update();

	fireball_powerup.update();
	if( fireball_powerup.is_active && fireball_powerup.timer->is_elapsed() )
	{
		fireball_powerup.deactivate( this );
	}
}

void e_ball::collided_with( ec_collider* collider, w_entity* entity_hit, c2Manifold& hit )
{
	assert( hit.count > 0 );	// sanity check

	if( entity_hit->collision_layer & cl_powup_multiball )
	{
		GAME->add_score( 100 );

		// destroy the brick
		entity_hit->set_life_cycle( life_cycle::dying );

		// play the pickup vfx
		auto efx = LAYER->add_entity<w_entity_fx>( entity_hit->pos, 0, 1 );
		efx->add_component<ec_emitter>()->init( "multiball_pickup" );
		efx->add_component<ec_sound>()->init( "powup_impact" );

		// spawn an extra ball at the same location as the brick
		auto ball = LAYER->add_entity<e_ball>( entity_hit->pos, 0.0f, scale );
	}
	else if( entity_hit->collision_layer & cl_powup_fireball )
	{
		GAME->add_score( 100 );

		// destroy the brick
		entity_hit->set_life_cycle( life_cycle::dying );

		fireball_powerup.activate( this );

		// play the pickup vfx
		auto efx = LAYER->add_entity<w_entity_fx>( entity_hit->pos, 0, 1 );
		efx->add_component<ec_emitter>()->init( "fireball_pickup" );
		efx->add_component<ec_sound>()->init( "powup_impact" );
	}
	else if( entity_hit->collision_layer & cl_brick )
	{
		GAME->add_score( 10 );

		entity_hit->set_life_cycle( life_cycle::dying );

		if( !fireball_powerup.is_active )
		{
			collider->push_outside( hit );

			// default ball collision behavior is to reflect off, maintaining speed

			w_vec2 forces_vec = w_vec2::normalize( physics_cache.forces );
			w_vec2 reflected_dir = w_vec2::reflect( forces_vec, w_vec2( hit.n.x, hit.n.y ) );

			// reset the force on the ball so it's using the new direction

			fudge_movement_dir( reflected_dir );

			auto force = get_component<ec_force_constant>();
			force->angle = w_vec2::to_angle( reflected_dir );
		}
		else
		{
			// 10 extra points for burning bricks
			GAME->add_score( 10 );
		}

		auto e = LAYER->add_entity<w_entity_fx>( pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "brick_impact" );
		e->add_component<ec_emitter>()->init( "brick_hit" );
	}
	else if( entity_hit->collision_layer & cl_deathzone )
	{
		set_life_cycle( life_cycle::dying );

		auto e = LAYER->add_entity<w_entity_fx>( pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "deathzone_impact" );
		e->add_component<ec_emitter>()->init( "ball_in_deathzone" );

		// count up remaining balls.  if zero, game is over.
		bool balls_remaining = false;

		for( auto& ent : LAYER->entities )
		{
			if( ent->collision_layer == cl_ball )
			{
				balls_remaining = true;
				break;
			}
		}

		if( !balls_remaining  )
		{
			engine->layer_mgr->push( std::make_unique<layer_firetoplay>() );
			GAME->snd_game_over->play();
		}
	}
	else if( entity_hit->collision_layer & cl_paddle )
	{
		collider->push_outside( hit );

		auto force = get_component<ec_force_constant>();
		w_vec2 new_dir = w_vec2::subtract( pos, entity_hit->pos ).normalize();
		force->angle = w_vec2::to_angle( new_dir );

		add_component<ec_force_multiplier>()->init( 0.5f, 2000 );

		auto e = LAYER->add_entity<w_entity_fx>( pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "paddle_impact" );
		e->add_component<ec_emitter>()->init( "brick_hit" );
	}
	else if( entity_hit->collision_layer & cl_ball )
	{
		collider->push_outside( hit );

		// 2 balls colliding should richochet away from each other

		auto force1 = get_component<ec_force_constant>();
		auto force2 = entity_hit->get_component<ec_force_constant>();

		std::swap( force1->angle, force2->angle );

		GAME->add_score( 10 );

		auto e = LAYER->add_entity<w_entity_fx>( pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "ball_impact" );
		e->add_component<ec_emitter>()->init( "balls_collide" );
	}
	else
	{
		collider->push_outside( hit );

		// default ball collision behavior is to reflect off, maintaining speed

		w_vec2 forces_vec = w_vec2::normalize( physics_cache.forces );
		w_vec2 reflected_dir = w_vec2::reflect( forces_vec, w_vec2( hit.n.x, hit.n.y ) );

		// reset the force on the ball so it's using the new direction
		auto force = get_component<ec_force_constant>();
		fudge_movement_dir( reflected_dir );
		force->angle = w_vec2::to_angle( reflected_dir );

		auto e = LAYER->add_entity<w_entity_fx>( pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "wall_impact" );
	}
 }

// ----------------------------------------------------------------------------

e_brick::e_brick()
{
	add_component<ec_collider>()->init_as_box( w_rect( -24, -12, 48, 24 ) );

	collision_layer = cl_brick;
	collides_with = cl_deathzone;
}

void e_brick::collided_with( ec_collider* collider, w_entity* entity_hit, c2Manifold& hit )
{
	assert( hit.count > 0 );	// sanity check

	if( entity_hit->collision_layer & cl_deathzone )
	{
		engine->layer_mgr->push( std::make_unique<layer_firetoplay>() );
		GAME->snd_game_over->play();
	}
}

// ----------------------------------------------------------------------------

e_brick_fireball::e_brick_fireball()
	: w_entity()
{
	collision_layer = cl_brick | cl_powup_fireball;

	add_component<ec_sprite>()->init( "brick_red" );
	add_component<ec_collider>()->init_as_box( w_rect( -24, -12, 48, 24 ) );
}

// ----------------------------------------------------------------------------

e_brick_multiball::e_brick_multiball()
{
	collision_layer = cl_brick | cl_powup_multiball;

	add_component<ec_sprite>()->init( "brick_yellow" );
	add_component<ec_collider>()->init_as_box( w_rect( -24, -12, 48, 24 ) );
}

// ----------------------------------------------------------------------------
