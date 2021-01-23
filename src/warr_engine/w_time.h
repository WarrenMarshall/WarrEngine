#pragma once

// NOTE : we can't name this file "time.h" because that confuses other includes

struct w_time
{
	int delta_ms = 0;

	// fixed time step accumulator. this accrues each frame until it exceeds
	// FTS_step_value_ms - then we call update() until it is below the threshold
	// again.

	int fts_accum_ms = 0;

	// used to change the speed of time passing. lower values are slower, higher
	// is faster.
	//
	// 0.25 = 25% of normal
	// 1.0 = normal speed (default)
	// 2.0 = 200% of normal

	float dilation = 1.0f;

	int prev_frame_ms = 0;

	w_time();
	void init();
	void update();
	[[nodiscard]] int get_time_ms() const;
};
