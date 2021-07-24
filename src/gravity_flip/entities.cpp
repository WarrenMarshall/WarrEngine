
#include "app_header.h"

using namespace war;

void e_player::on_touched( simple_collision::pending_collision& coll )
{
	if( auto mc = get_component<ec_movement_controller>() ; mc )
	{
		mc->on_touched( coll );
	}
}
