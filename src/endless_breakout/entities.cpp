
#include "app_header.h"

// ----------------------------------------------------------------------------

e_player::e_player()
{
	add_component<ec_sprite>()->init( "sub_tex_paddle_blue" );
}

// ----------------------------------------------------------------------------

e_ball::e_ball()
{
	add_component<ec_sprite>()->init( "sub_tex_ball_blue" );
}

void e_ball::update()
{
	w_entity::update();
}

void e_ball::collided_with( w_entity* entity_hit, c2Manifold& manifold )
{
	assert( manifold.count > 0 );	// sanity check

	// resolve the ball position so it's outside the collider
	w_vec2 n = w_vec2( manifold.n.x * -1.0f, manifold.n.y * -1.0f );
	pos.x += n.x * ( 1.0f - manifold.depths[ 0 ] );
	pos.y += n.y * ( 1.0f - manifold.depths[ 0 ] );

	// compute a reflection vector based on the travel direction and
	// the normal of the collision
	glm::vec3 dir = ( glm::vec3 )w_vec2::normalize( physics_cache.forces );
	glm::vec3 normal( manifold.n.x, manifold.n.y, 0.0f );
	glm::vec3 reflected_dir = glm::reflect( dir, normal );

	// reset the force on the ball so it's using the new direction
	w_force* force = forces.back().get();
	force->dir.x = reflected_dir.x;
	force->dir.y = reflected_dir.y;
}
