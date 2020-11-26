
#include "app_header.h"

// ----------------------------------------------------------------------------

w_twinstick_physics_responder::w_twinstick_physics_responder()
	: w_physics_responder()
{
}

void w_twinstick_physics_responder::BeginContact( b2Contact* contact )
{
	w_physics_responder::BeginContact( contact );

}

#if 0
void w_twinstick_physics::update()
{
}
#endif
