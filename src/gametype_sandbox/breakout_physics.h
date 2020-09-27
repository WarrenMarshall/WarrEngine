#pragma once

// ----------------------------------------------------------------------------

struct w_breakout_physics : w_contact_listener, w_physics
{
	w_breakout_physics();

	virtual void BeginContact( b2Contact* contact ) override;
	//virtual void EndContact( b2Contact* contact ) override;
};
