#pragma once

struct layer_pong : w_layer
{
	std::unique_ptr<w_pong_physics> pong_physics = nullptr;

	layer_pong();

	w_entity* ball = nullptr;
	w_entity* world_geo = nullptr;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
};
