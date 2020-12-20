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
    void accum( float value );
    void inc();
	void dec();

    float _accumulator;
};
