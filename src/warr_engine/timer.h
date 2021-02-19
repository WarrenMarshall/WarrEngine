#pragma once

struct w_timer
{
	time_ms time_last = 0;
	time_ms interval_ms = 0;		// how many ms before this timer elapses

	time_ms delta_accum = 0;

	w_timer() = default;
	w_timer( time_ms interval_ms );

	void restart();
	[[nodiscard]] bool is_elapsed();
	[[nodiscard]] int get_elapsed();
};
