
namespace war
{

struct Timer final
{
	time_ms time_last = 0;
	time_ms interval_ms = 0;		// how many ms before this timer elapses

	time_ms delta_accum = 0;

	Timer() = default;
	Timer( time_ms interval_ms );

	void restart();
	[[nodiscard]] bool is_elapsed();
	[[nodiscard]] int32_t get_elapsed();
	[[nodiscard]] float_t get_pct_complete();
};

}
