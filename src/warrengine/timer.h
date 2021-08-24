
namespace war
{

struct timer final
{
	time_ms time_last = 0;
	time_ms interval_ms = 0;		// how many ms before this timer elapses

	time_ms delta_accum = 0;

	timer() = default;
	timer( time_ms interval_ms );

	void restart();
	[[nodiscard]] bool is_elapsed();
	[[nodiscard]] int get_elapsed();
	[[nodiscard]] float get_pct_complete();
};

}
