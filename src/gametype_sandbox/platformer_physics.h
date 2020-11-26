#pragma once

// ----------------------------------------------------------------------------

struct w_platformer_contact_listener : w_physics_responder
{
	w_platformer_contact_listener();

	virtual void BeginContact( b2Contact* contact ) override;
	virtual void EndContact( b2Contact* contact ) override;

	std::unique_ptr<w_timer> timer_jump_limiter = nullptr;
	int player_on_ground = 0;
	int player_drop_down_blocked = 0;

	bool can_jump();
	bool in_air();
	bool can_drop_down();
	//void handle_user_input( w_entity* player ) override;

	//virtual bool event_input_motion( const w_input_event* evt ) override;
	//virtual bool event_input_pressed( const w_input_event* evt ) override;
	//virtual bool event_input_held( const w_input_event* evt ) override;
	//virtual bool event_input_released( const w_input_event* evt ) override;

	//virtual void update() override;
};
