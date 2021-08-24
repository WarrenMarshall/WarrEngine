
namespace war
{

struct render_stats final
{
	void init();

	timer stat_timer;

	value_accumulator frame_count;

	value_accumulator draw_calls;
	value_accumulator frame_times_ms;
	value_accumulator entities;
	value_accumulator quads;
	value_accumulator triangles;
	value_accumulator lines;
	value_accumulator points;

	std::string stat_custom_string;
	std::vector<std::string> stat_strings;
	bool draw_verbose = false;

	void update();
	void draw();
};

}
