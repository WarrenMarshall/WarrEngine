#pragma once

struct breakout_layer : w_layer
{
	std::unique_ptr<w_breakout_physics> pong_physics = nullptr;

	e_breakout_paddle* player = nullptr;

	breakout_layer();

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;

	void spawn_ball();
	virtual bool handle_input_event( const w_input_event* evt ) override;
};
