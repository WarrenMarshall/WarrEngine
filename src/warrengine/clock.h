
namespace war
{

struct Clock
{
	// the time when this frame started
	time_ms current_frame_ms = 0;
	time_ms prev_frame_ms = 0;

	// the time that has elapsed since the last frame.
	float delta_ms = 0;

	// the time that has elapsed since the app started.
	double time_since_app_start_ms = 0;

	// fixed time step accumulator. this accrues each frame until it exceeds
	// FTS_step_value_ms - then we call update() until it is below the threshold
	// again.
	float fts_accum_ms = 0;

	// used to change the speed of time passing. lower values are slower, higher
	// is faster.
	//
	// 0.25 = 25% of normal
	// 1.0 = normal speed (default)
	// 2.0 = 200% of normal
	float dilation = 1.f;

	Clock();
	void init();
	void update();
	[[nodiscard]] time_ms now() const;

private:
	[[nodiscard]] time_ms get_ticks();
};

}
