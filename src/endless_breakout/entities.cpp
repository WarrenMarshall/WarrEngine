
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

void e_ball::collided_with( w_entity* entity_hit, c2Manifold& hit )
{
	assert( hit.count > 0 );	// sanity check

	// compute a reflection vector based on the travel direction and
	// the normal of the collision
	glm::vec3 forces_vec = ( glm::vec3 )w_vec2::normalize( physics_cache.forces );
	glm::vec3 hit_normal( hit.n.x, hit.n.y, 0.0f );
	glm::vec3 reflected_dir = glm::reflect( forces_vec, hit_normal );

	// reset the force on the ball so it's using the new direction
	w_force* force = forces.back().get();
	force->dir.x = reflected_dir.x;
	force->dir.y = reflected_dir.y;
}
