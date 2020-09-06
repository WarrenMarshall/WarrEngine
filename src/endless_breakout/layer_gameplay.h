#pragma once

struct layer_gameplay : w_layer
{
	w_entity* paddle = nullptr;
	w_entity* ball = nullptr;
	unsigned new_row_spawn_countdown = 0;

	virtual void push() final;
	virtual void becoming_top_layer() final;
	virtual void getting_covered() final;
	virtual void update() final;
	virtual void draw() final;

	virtual bool handle_input_event( const w_input_event* evt ) final;
};
