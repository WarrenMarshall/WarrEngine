#pragma once

struct w_timer
{
	float time_remaining_ms = 0;// how many ms are left before this timer elapses
	float interval_ms = 0;		// how many ms before this timer elapses

	w_timer( int interval_ms );

	void reset();
	void update();
	[[nodiscard]] bool is_elapsed() const;
	[[nodiscard]] int get_elapsed_count();
};
