#include "app_header.h"

using namespace war;

E_Player::E_Player()
{
	{
		auto ec = add_component<Simple_Collision_Body>();
		ec->set_as_centered_box( 8, 16 );
		ec->flags.draw_as_shape = true;
	}
}
