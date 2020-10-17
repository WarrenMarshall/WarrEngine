#pragma once

// ----------------------------------------------------------------------------

struct w_platformer_physics : w_contact_listener, w_physics
{
	w_platformer_physics();

	virtual void BeginContact( b2Contact* contact ) override;
	virtual void EndContact( b2Contact* contact ) override;

	std::unique_ptr<w_timer> timer_jump_limiter = nullptr;
	int player_on_ground = 0;
	int player_drop_down_blocked = 0;

	bool can_jump();
	bool in_air();
	bool on_ground();
	bool can_drop_down();
	void hit_ground();
	void handle_user_input();

	virtual void update() override;
};
