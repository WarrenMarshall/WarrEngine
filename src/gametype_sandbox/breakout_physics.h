#pragma once

// ----------------------------------------------------------------------------

struct w_breakout_contact_listener : w_physics_responder
{
	w_breakout_contact_listener();

	virtual void BeginContact( b2Contact* contact ) override;
	//virtual void EndContact( b2Contact* contact ) override;
};
