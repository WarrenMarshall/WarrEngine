
#include "app_header.h"

// ----------------------------------------------------------------------------

void e_emoji_ball::on_box2d_collision_begin( box2d_physics::Pending_Collision& coll, Entity* other )
{
	if( other->tag == H( "world" ) )
	{
		g_engine->find_asset<Sound_Asset>( "sfx_bump" )->play();
	}
}
