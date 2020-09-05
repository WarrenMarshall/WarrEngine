
#include "app_header.h"

// ----------------------------------------------------------------------------

e_paddle::e_paddle()
{
	add_component<ec_sprite>()->init( "sub_tex_paddle_blue" );
	add_component<ec_collider>()->init_as_box( w_rect( -48, -8, 96, 16 ) );
	//debug_draw_collision = true;

	collision_layer = cl_paddle;
	collides_with = 0;
}

// ----------------------------------------------------------------------------

e_ball::e_ball()
{
	add_component<ec_sprite>()->init( "sub_tex_ball_blue" );
	add_component<ec_collider>()->init_as_circle( 7 );
	forces.emplace_back( std::make_unique<w_force>( w_vec2::get_random_unit(), 150.0f ) );

	collision_layer = cl_ball;
	collides_with = cl_ball | cl_wall | cl_brick | cl_paddle | cl_deathzone;
}

void e_ball::collided_with( w_entity* entity_hit, c2Manifold& hit )
{
	assert( hit.count > 0 );	// sanity check

	if( entity_hit->collision_layer == cl_brick )
	{
		GAME->sfx_brick_impact->play();

		entity_hit->set_life_cycle( lifecycle::dying );

		// default ball collision behavior is to reflect off, maintaining speed

		glm::vec3 forces_vec = ( glm::vec3 )w_vec2::normalize( physics_cache.forces );
		glm::vec3 hit_normal( hit.n.x, hit.n.y, 0.0f );
		glm::vec3 reflected_dir = glm::reflect( forces_vec, hit_normal );

		// reset the force on the ball so it's using the new direction
		w_force* force = forces.back().get();
		force->dir.x = reflected_dir.x;
		force->dir.y = reflected_dir.y;
	}
	else if( entity_hit->collision_layer == cl_deathzone )
	{
		GAME->sfx_deathzone_impact->play();

		set_life_cycle( lifecycle::dying );
	}
	else if( entity_hit->collision_layer == cl_paddle )
	{
		GAME->sfx_paddle_impact->play();

		w_force* force = forces.back().get();
		w_vec2 new_dir = w_vec2::subtract( pos, entity_hit->pos );
		force->dir = new_dir.normalize();
	}
	else if( entity_hit->collision_layer == cl_ball )
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
