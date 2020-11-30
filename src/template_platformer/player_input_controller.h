#pragma once

struct w_player_input_controller : i_input_receiver
{
	// custom data for this controller
	w_physics_responder* physics_responder = nullptr;

	virtual bool iir_on_motion( const w_input_event* evt ) override;
	virtual bool iir_on_pressed( const w_input_event* evt ) override;
};
