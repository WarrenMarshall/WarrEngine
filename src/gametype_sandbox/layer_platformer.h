#pragma once

struct layer_platformer : w_layer
{
	std::unique_ptr<w_platformer_physics> plat_physics = nullptr;

	layer_platformer();

	w_entity* player = nullptr;
	w_entity* player2 = nullptr;
	w_entity* world_geo = nullptr;

	virtual void push() override;
	virtual void pop() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool handle_input_event( const w_input_event* evt );
};
