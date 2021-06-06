
#include "app_header.h"

// ----------------------------------------------------------------------------

void e_emoji_ball::on_collision_begin( physics_pending_collision& coll, entity* other )
{
	if( other->tag == H( "world" ) )
	{
		g_engine->find_asset<sound_asset>( "sfx_bump" )->play();
	}
}
