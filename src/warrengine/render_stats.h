
namespace war
{

struct Render_Stats final
{
#ifndef _RELEASE

	Timer stat_timer;

	Value_Accumulator _frame_count;

	Value_Accumulator _draw_calls;
	Value_Accumulator _frame_times_ms;
	Value_Accumulator _entities;
	Value_Accumulator _quads;
	Value_Accumulator _triangles;
	Value_Accumulator _lines;
	Value_Accumulator _points;

	std::string stat_custom_string;
	std::vector<std::string> stat_strings;

	struct
	{
		// if true, displays the larger stats display at the top of the frame rather
		// than the single line showing the frame rate.
		bool draw_verbose : 1 = false;

		// if true, stats are not collected. this is useful for pausing stat
		// collection on screens or in drawing paths we don't want to track.
		bool pause : 1 = false;
	} flags;

#endif

	void init();
	void update();
	void draw();
	void add( Value_Accumulator& accum, float_t val );
	void inc( Value_Accumulator& accum );
};

}
