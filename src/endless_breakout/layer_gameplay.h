#pragma once

struct layer_gameplay : w_layer
{
	w_entity* walls = nullptr;
	w_entity* death_zone = nullptr;
	w_entity* player = nullptr;
	std::vector<w_entity*> balls;
	bool ball_attached_to_paddle = true;

	layer_gameplay();

	virtual void push() final;
	virtual void update_collisions() final;

	virtual bool handle_input_event( const w_input_event* evt ) final;
};
