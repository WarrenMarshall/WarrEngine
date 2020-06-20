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

struct w_render_state
{
	bool auto_activation = false;

	// used for timing temporary states
	int time_interval_ms = 0;
	float time_remaining_ms = 0;
	
	w_render_state( bool auto_activation );

	virtual void apply() {};
	virtual void remove() {};
	virtual void reset();
	virtual void update();
};

// ----------------------------------------------------------------------------

struct rs_color : w_render_state
{
	w_color saved_color = W_COLOR_WHITE;
	w_color new_color = W_COLOR_BLACK;

	rs_color( float r, float g, float b, bool auto_activation = true );
	rs_color( w_color new_color, bool auto_activation = true );
	~rs_color();

	virtual void apply();
	virtual void remove();
};

// ----------------------------------------------------------------------------

struct rs_scale : w_render_state
{
	float saved_scale = 1.0f;
	float new_scale = 1.0f;

	rs_scale( float new_scale, bool auto_activation = true );
	~rs_scale();

	virtual void apply();
	virtual void remove();
};

// ----------------------------------------------------------------------------

struct rs_angle : w_render_state
{
	float saved_angle = 0.0f;
	float new_angle = 0.0f;

	rs_angle( float new_angle, bool auto_activation = true );
	~rs_angle();

	virtual void apply();
	virtual void remove();
};

// ----------------------------------------------------------------------------

struct rs_alpha : w_render_state
{
	float saved_alpha = 1.0f;
	float new_alpha = 1.0f;

	rs_alpha( float new_alpha, bool auto_activation = true );
	~rs_alpha();

	virtual void apply();
	virtual void remove();
};
