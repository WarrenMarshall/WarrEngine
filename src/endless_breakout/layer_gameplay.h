#pragma once

struct layer_gameplay : w_layer
{
	w_entity* walls = nullptr;
	w_entity* player = nullptr;
	w_entity* ball = nullptr;
	bool ball_attached_to_paddle = true;

	layer_gameplay();

	virtual void push() override;

	virtual bool handle_input_event( const w_input_event* evt ) override;
};
