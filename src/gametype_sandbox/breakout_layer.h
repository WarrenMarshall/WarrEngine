#pragma once

struct breakout_layer : w_layer
{
	std::unique_ptr<w_breakout_physics_responder> breakout_physics = nullptr;

	e_breakout_paddle* player = nullptr;

	breakout_layer();

	virtual void push() override;
	virtual void pop() override;
	virtual void draw() override;

	void spawn_ball();
	virtual bool event_input_motion( const w_input_event* evt ) override;
	virtual bool event_input_released( const w_input_event* evt ) override;
};
