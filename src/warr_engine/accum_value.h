#pragma once

// ----------------------------------------------------------------------------
// a value that wants to accumulate over a period of time, and then report
// an averaged value for that period.

struct w_accum_value
{
	float value;

	void init();
	void update_value( int steps );
	void update_value();

	w_accum_value& operator++();
	w_accum_value operator++( int );
	w_accum_value& operator--();
	w_accum_value operator--( int );
	w_accum_value operator+( float value ) const;
	w_accum_value operator+=( float v );
	w_accum_value operator+( time_ms value ) const;
	w_accum_value operator+=( time_ms v );

	float _accumulator;
};
