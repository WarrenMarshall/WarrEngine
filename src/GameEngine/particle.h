#pragma once

struct w_particle : i_transform
{
	a_texture* tex = nullptr;

	// how long this particle should live, in ticks
	float life_span = 0.0f;
	float life_span_save = 0.0f;	// saves the original life span for reference

	// angle of movement
	float a_dir = 0.0f;

	// world units to move, per second
	float velocity_per_sec = 0.0f;

	// current particle state
	w_timeline* t_scale = nullptr;
	float base_scale = 0.0f;
	w_timeline* t_alpha = nullptr;
	w_timeline* t_color = nullptr;

	float spin = 0.0f;
	float spin_per_sec = 0.0f;

	w_particle();

	void update();
	bool is_alive();
	void init_to_new();
};
