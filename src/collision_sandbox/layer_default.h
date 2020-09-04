#pragma once

struct layer_default : w_layer
{
	w_entity* player = nullptr;
	ec_collider* player_ec = nullptr;

	w_entity* blocker = nullptr;
	ec_collider* blocker_ec = nullptr;

	std::string status;
	c2Manifold hit;

	layer_default();

	virtual void push() final;
	virtual void update() final;
	virtual void draw() final;
	virtual bool handle_input_event( const w_input_event* evt ) final;
};
