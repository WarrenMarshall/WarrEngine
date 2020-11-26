#pragma once

// ----------------------------------------------------------------------------

struct w_breakout_physics_responder : w_physics_responder
{
	w_breakout_physics_responder();

	virtual void BeginContact( b2Contact* contact ) override;
	//virtual void EndContact( b2Contact* contact ) override;
};
