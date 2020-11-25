#pragma once

struct w_render_stats
{
	w_render_stats();

	std::unique_ptr<w_timer> stat_timer = nullptr;

	w_accum_value frame_count;

	w_accum_value render_buffers;
	w_accum_value render_vertices;
	w_accum_value render_indices;
	w_accum_value frame_times_ms;
	w_accum_value num_entities;

	std::string stat_custom_string;

	void update();
};

