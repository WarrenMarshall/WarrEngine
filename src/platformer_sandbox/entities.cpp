
#include "app_header.h"
#include "entities.h"

void e_player::collided_with( ec_collider* collider, w_entity* entity_hit, c2Manifold& hit )
{
	if( entity_hit->collision_layer & cl_world )
	{
		auto force_list = get_components<ec_force_dir_accum>();

		collider->push_outside( hit, 0.0f );

		for( auto& f : force_list )
		{
			f->strength = 0.0f;
		}
	}
}
