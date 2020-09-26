#pragma once

struct layer_pong : w_layer
{
	std::unique_ptr<w_pong_physics> pong_physics = nullptr;

	e_pong_paddle* player = nullptr;

	layer_pong();

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;

	void spawn_ball();
	virtual bool handle_input_event( const w_input_event* evt ) override;
};
