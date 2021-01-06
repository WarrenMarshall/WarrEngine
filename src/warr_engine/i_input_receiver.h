#pragma once

// #shallow : kill this base class and move the 4 virtual functions into any class that needs them
struct i_input_receiver
{
	virtual bool iir_on_motion( const w_input_event* evt );
	virtual bool iir_on_pressed( const w_input_event* evt );
	virtual bool iir_on_held( const w_input_event* evt );
	virtual bool iir_on_released( const w_input_event* evt );
};
