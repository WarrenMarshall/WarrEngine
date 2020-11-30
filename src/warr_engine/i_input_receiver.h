#pragma once

struct i_input_receiver
{
	virtual bool iir_on_motion( const w_input_event* evt );
	virtual bool iir_on_pressed( const w_input_event* evt );
	virtual bool iir_on_held( const w_input_event* evt );
	virtual bool iir_on_released( const w_input_event* evt );
};
