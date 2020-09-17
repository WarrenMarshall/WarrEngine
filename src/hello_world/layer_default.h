#pragma once

struct layer_default : w_layer
{
	layer_default();

	//w_entity* player;
	w_entity* world_geo;

	b2Body* dynamic_body;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
};
