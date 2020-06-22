#pragma once

/*
	render states are temporary structs you create inside of a context
	to control a rendering state.

	for example, to flip all rendering in a context horizontally
	and set the alpha to 64:

	{
		SCOPED_VAR( rs_alpha( 64 ) );
		.
		. draw calls here
		.
	}

	when this context exits, the temporary object is deleted and it's
	destructor resets whatever it changed back to the original.
*/

struct i_render_state
{
	virtual void push() = 0;
	virtual void pop() = 0;
};

// ----------------------------------------------------------------------------

struct rs_color : i_render_state
{
	w_color saved_color = W_COLOR_WHITE;
	w_color new_color = W_COLOR_BLACK;

	rs_color( float r, float g, float b);
	rs_color( w_color new_color );
	~rs_color();

	virtual void push();
	virtual void pop();
};
