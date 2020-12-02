#pragma once

struct w_player_input_receiver : i_input_receiver
{
	virtual bool iir_on_motion( const w_input_event* evt ) override;
	virtual bool iir_on_pressed( const w_input_event* evt ) override;
};
