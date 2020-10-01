#pragma once

// ----------------------------------------------------------------------------

struct w_twinstick_physics : w_contact_listener, w_physics
{
	static float player_move_force_s;
	static float player_base_radius;
	static float player_move_force_max;

	w_twinstick_physics();

	virtual void BeginContact( b2Contact* contact ) override;
	void handle_user_input( w_entity* player );
	//virtual void EndContact( b2Contact* contact ) override;
};
