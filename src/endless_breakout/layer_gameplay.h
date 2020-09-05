#pragma once

struct layer_gameplay : w_layer
{
	w_entity* paddle = nullptr;
	std::unique_ptr<w_timer> timer_brick_move;
	unsigned new_row_spawn_countdown = 0;

	layer_gameplay();

	virtual void push() final;
	virtual void update() final;
	virtual void update_collisions() final;

	virtual bool handle_input_event( const w_input_event* evt ) final;
};
