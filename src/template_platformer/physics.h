#pragma once

// ----------------------------------------------------------------------------

struct w_platformer_physic_responder : w_physics_responder
{
	w_platformer_physic_responder();

	virtual void BeginContact( b2Contact* contact ) override;
	virtual void EndContact( b2Contact* contact ) override;

	std::unique_ptr<w_timer> timer_jump_limiter = nullptr;
	byte player_on_ground = 0;
	byte player_drop_down_blocked = 0;
	float vel_x = 0.0f;
	float vel_y = 0.0f;

	[[nodiscard]] bool can_jump() const;
	[[nodiscard]] bool in_air() const;
	[[nodiscard]] bool on_ground() const;
	[[nodiscard]] bool can_drop_down() const;
	void hit_ground();
	void handle_user_input();

	virtual void update() override;
};
