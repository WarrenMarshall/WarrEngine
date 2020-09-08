
#include "app_header.h"

void e_player::collided_with( ec_collider* collider, w_entity* entity_hit, c2Manifold& hit )
{
	if( entity_hit->collision_layer & cl_world )
	{
		collider->push_outside( hit );
	}
}
