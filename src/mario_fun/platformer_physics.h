#pragma once

// ----------------------------------------------------------------------------

struct w_platformer_physics : w_contact_listener, w_physics
{
	w_platformer_physics();

	virtual void BeginContact( b2Contact* contact ) override;
	virtual void EndContact( b2Contact* contact ) override;

	std::unique_ptr<w_timer> timer_jump_limiter = nullptr;
	byte player_on_ground = 0;
	byte player_drop_down_blocked = 0;

	_NODISCARD bool can_jump() const;
	_NODISCARD bool in_air() const;
	_NODISCARD bool on_ground() const;
	_NODISCARD bool can_drop_down() const;
	void hit_ground();
	void handle_user_input();

	virtual void update() override;
	virtual bool handle_input_event( const w_input_event* evt ) override;
};
