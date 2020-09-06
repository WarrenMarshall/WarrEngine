
#include "app_header.h"

// ----------------------------------------------------------------------------

e_paddle::e_paddle()
{
	add_component<ec_sprite>()->init( "sub_tex_paddle" );
	add_component<ec_collider>()->init_as_box( w_rect( -48, -8, 96, 16 ) );

	collision_layer = cl_paddle;
	collides_with = 0;
}

// ----------------------------------------------------------------------------

e_ball::e_ball()
{
	add_component<ec_sprite>()->init( "sub_tex_ball" );
	add_component<ec_collider>()->init_as_circle( 7 );
	ec_fire_trail = add_component<ec_emitter>()->init( "fireball_trail" );
	ec_fire_trail->active = false;
	forces.emplace_back( std::make_unique<w_force>( w_vec2::get_random_unit(), 150.0f ) );

	collision_layer = cl_ball;
	collides_with = cl_ball | cl_wall | cl_brick | cl_paddle | cl_deathzone | cl_powup_multiball | cl_powup_fireball;
}

void e_ball::collided_with( w_entity* entity_hit, c2Manifold& hit )
{
	assert( hit.count > 0 );	// sanity check

	auto layer = engine->layer_mgr->get_top();

	if( entity_hit->collision_layer & cl_powup_multiball )
	{
		// destroy the brick
		entity_hit->set_life_cycle( lifecycle::dying );

		// play the pickup vfx
		auto e = layer->spawn_entity<w_entity_cozy>();
		e->set_transform( entity_hit->pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "powup_impact" );
		e->add_component<ec_emitter>()->init( "powup_pickup" );

		// spawn an extra ball at the same location as the brick
		auto ball = layer->spawn_entity<e_ball>( entity_hit->pos, 0.0f, scale );
	}
	else if( entity_hit->collision_layer & cl_powup_fireball )
	{
		// destroy the brick
		entity_hit->set_life_cycle( lifecycle::dying );

		// turn on the fire trail for the ball
		ec_fire_trail->active_time_remaining_ms += 5000;

		// play the pickup vfx
		auto e = layer->spawn_entity<w_entity_cozy>();
		e->set_transform( entity_hit->pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "powup_impact" );
		e->add_component<ec_emitter>()->init( "powup_pickup" );
	}
	else if( entity_hit->collision_layer & cl_brick )
	{
		GAME->sfx_brick_impact->play();

		entity_hit->set_life_cycle( lifecycle::dying );

		if( !ec_fire_trail->is_active() )
		{
			// default ball collision behavior is to reflect off, maintaining speed

			glm::vec3 forces_vec = ( glm::vec3 )w_vec2::normalize( physics_cache.forces );
			glm::vec3 hit_normal( hit.n.x, hit.n.y, 0.0f );
			glm::vec3 reflected_dir = glm::reflect( forces_vec, hit_normal );

			// reset the force on the ball so it's using the new direction
			w_force* force = forces.back().get();
			force->dir.x = reflected_dir.x;
			force->dir.y = reflected_dir.y;
		}
	}
	else if( entity_hit->collision_layer & cl_deathzone )
	{
		set_life_cycle( lifecycle::dying );

		auto e = layer->spawn_entity<w_entity_cozy>();
		e->set_transform( pos, 0.0f, 1.0f );
		e->add_component<ec_sound>()->init( "deathzone_impact" );
		e->add_component<ec_emitter>()->init( "ball_in_deathzone" );
	}
	else if( entity_hit->collision_layer & cl_paddle )
	{
		GAME->sfx_paddle_impact->play();

		w_force* force = forces.back().get();
		w_vec2 new_dir = w_vec2::subtract( pos, entity_hit->pos );
		force->dir = new_dir.normalize();
	}
	else if( entity_hit->collision_layer & cl_ball )
	{
		GAME->sfx_ball_impact->play();

		// 2 balls colliding should richochet away from each other

		w_force* force1 = forces.back().get();
		w_force* force2 = entity_hit->forces.back().get();

		std::swap( force1->dir, force2->dir );
	}
	else
	{
		GAME->sfx_wall_impact->play();

		// default ball collision behavior is to reflect off, maintaining speed

		glm::vec3 forces_vec = ( glm::vec3 )w_vec2::normalize( physics_cache.forces );
		glm::vec3 hit_normal( hit.n.x, hit.n.y, 0.0f );
		glm::vec3 reflected_dir = glm::reflect( forces_vec, hit_normal );

		// reset the force on the ball so it's using the new direction
		w_force* force = forces.back().get();
		force->dir.x = reflected_dir.x;
		force->dir.y = reflected_dir.y;
	}
}

// ----------------------------------------------------------------------------

e_brick::e_brick()
{
	add_component<ec_collider>()->init_as_box( w_rect( -24, -12, 48, 24 ) );

	collision_layer = cl_brick;
}

// ----------------------------------------------------------------------------

e_brick_fireball::e_brick_fireball()
	: w_entity()
{
	collision_layer = cl_brick | cl_powup_fireball;

	add_component<ec_sprite>()->init( "sub_brick_red" );
	add_component<ec_collider>()->init_as_box( w_rect( -24, -12, 48, 24 ) );
}
// ----------------------------------------------------------------------------

e_brick_multiball::e_brick_multiball()
{
	collision_layer = cl_brick | cl_powup_multiball;

	add_component<ec_sprite>()->init( "sub_brick_yellow" );
	add_component<ec_collider>()->init_as_box( w_rect( -24, -12, 48, 24 ) );
}
