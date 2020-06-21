#pragma once

struct w_time
{
	// set to however many times per second you want the logic updated
	static const float FTS_desired_frames_per_second;
	// how large a time step in fixed time is - computed automatically
	static const float FTS_step_value_ms;

	float delta_ms = 0.0f;
	float delta_s = 0.0f;
	uint64_t prev_frame_ms = 0;

	// fixed time step accumulator. this accrues each frame
	// until it exceeds FTS_step_value_ms - then we call
	// update() until it is below the threshold again.
	float fts_accum_ms = 0;

	/*
		used to change the speed of time passing.
		lower values are slower, higher is faster.

		0.25 = 25% of normal
		1.0 = normal speed (default)
		2.0 = 200% of normal
	*/
	float dilation = 1.0f;
	
	w_time();
	void init();
	void update();
	uint64_t get_ticks();
};
