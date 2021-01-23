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
	void accum( time_ms value );
	void inc();
	void dec();

	float _accumulator;
};

// ----------------------------------------------------------------------------

template <typename T>
struct w_lerp_value
{
	T start, end;		// range of values
	float duration_ms;	// how long it should take, end to end
	bool dir;			// which way we're going, true:(start -> end) / false:(end -> start)

	float time_accum;	// how much time has passed since we started

	w_lerp_value<T>()
	{
		start = end = {};
		duration_ms = 0.0f;
		dir = true;
		time_accum = 0.0f;
	}

	w_lerp_value<T>( T start, T end, float duration )
		: start( start ), end( end ), duration_ms( duration )
	{
		dir = true;
		time_accum = 0.0f;
	}

	//T operator*()
	//{
	//	return (end - start)
	//}
};
