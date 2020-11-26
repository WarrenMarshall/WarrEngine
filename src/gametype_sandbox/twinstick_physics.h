#pragma once

// ----------------------------------------------------------------------------

struct w_twinstick_contact_listener : w_physics_responder
{
	static float player_move_force_s;
	static float player_base_radius;
	static float player_move_force_max;

	w_vec2 trace_hit_location = w_vec2::zero;

	w_twinstick_contact_listener();

	virtual void BeginContact( b2Contact* contact ) override;
	//virtual void EndContact( b2Contact* contact ) override;

	void handle_user_input( w_entity* player );
};
