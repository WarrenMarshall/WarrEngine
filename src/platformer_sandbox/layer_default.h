#pragma once

struct layer_default : w_layer
{
	e_player* player = nullptr;
	w_entity* world_geo = nullptr;

	ec_force_constant* efc_player_input = nullptr;

	layer_default();

	virtual void push() final;
	virtual void update_physics() final;
	virtual void update() final;
	virtual bool handle_input_event( const w_input_event* evt ) final;
};
