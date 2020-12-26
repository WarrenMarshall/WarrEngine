#pragma once

struct a_emitter_params : i_asset
{
	declare_find_func( a_emitter_params )

	// texture to draw for this particle
	a_texture* tex = nullptr;

	// where to spawn new particles
	std::unique_ptr<w_particle_spawner> particle_spawner = nullptr;
	std::unique_ptr<w_timeline> t_scale = nullptr;

	w_range r_velocity_spawn = w_range( 50, 50 );	// initial velocity
	w_range r_scale_spawn = w_range( 1, 1 );		// scale - spawn value and timeline
	w_range r_lifespan = w_range( 500, 1000 );		// lifetime in milliseconds
	w_range r_spin_per_sec = w_range( 0, 0 );		// ranges for controlling angle values
	w_range r_spin_spawn = w_range( 0, 0 );
	w_range r_dir_var = w_range( 0, 0 );			// variance for the spawn direction

	// timelines for interpolating color and alpha
	std::unique_ptr<w_timeline> t_color = nullptr;
	std::unique_ptr<w_timeline> t_alpha = nullptr;

	float s_max_spawn_per_sec = 0.0f;	// how many particles to spawn from this emitter, per second
	float a_dir = 0.0f;					// the base direction that particles start moving in when they spawn

	struct
	{
		char needs_warm_up : 1;
		char is_one_shot : 1;
	};

	a_emitter_params();
};

