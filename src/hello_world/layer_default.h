#pragma once

struct layer_default : w_layer
{
	layer_default();

	w_entity* player = nullptr;
	w_entity* world_geo = nullptr;

	virtual void push() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool handle_input_event( const w_input_event* evt );
};
