#pragma once

struct layer_pong : w_layer
{
	w_contact_listener contact_listener;
	w_vec2 player_trace_hit;
	bool player_on_ground = true;

	layer_pong();

	w_entity* ball = nullptr;
	w_entity* world_geo = nullptr;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
};
