#pragma once

struct w_timer
{
	time_ms time_last = 0;
	time_ms time_remaining_ms = 0;	// how many ms are left before this timer elapses
	time_ms interval_ms = 0;		// how many ms before this timer elapses

	w_timer() = default;
	w_timer( time_ms interval_ms );

	void restart();
	[[nodiscard]] bool is_elapsed() const;
	[[nodiscard]] int get_elapsed_count();
};
