
#include "app_header.h"

// ----------------------------------------------------------------------------

// #warren - move this into the scene file for handling, and remove this h/cpp

void E_Emoji_Ball::on_box2d_collision_begin( box2d::Pending_Collision& coll, Entity* other )
{
	if( other->tag == H( "world" ) )
	{
		g_engine->find_asset<Sound_Asset>( "sfx_bump" )->play();
	}
}
