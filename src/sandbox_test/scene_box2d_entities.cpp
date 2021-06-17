
#include "app_header.h"

// ----------------------------------------------------------------------------

void e_emoji_ball::on_box2d_collision_begin( box2d_physics::pending_collision& coll, entity* other )
{
	if( other->tag == H( "world" ) )
	{
		g_engine->find_asset<sound_asset>( "sfx_bump" )->play();
	}
}
