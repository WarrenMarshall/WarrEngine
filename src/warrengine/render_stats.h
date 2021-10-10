
namespace war
{

struct Render_Stats final
{
#if !defined(_FINAL_RELEASE)

	Timer stat_timer;

	Value_Accumulator frame_count;

	Value_Accumulator draw_calls;
	Value_Accumulator frame_times_ms;
	Value_Accumulator entities;
	Value_Accumulator quads;
	Value_Accumulator triangles;
	Value_Accumulator lines;
	Value_Accumulator points;

	std::string stat_custom_string;
	std::vector<std::string> stat_strings;
	bool draw_verbose = false;
#endif

	void init();
	void update();
	void draw();
};

}
