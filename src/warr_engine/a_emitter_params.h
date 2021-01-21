#pragma once

struct a_emitter_params : i_asset
{
	declare_find_func( a_emitter_params )

	// texture to draw for this particle
	a_texture* texture = nullptr;
	w_timeline t_color = w_timeline( timeline_type::color_type );
	w_timeline t_alpha = w_timeline( timeline_type::float_type );
	w_timeline t_scale = w_timeline( timeline_type::float_type );
	// #cache
	std::unique_ptr<w_particle_spawner> particle_spawner = nullptr;

	w_range r_velocity_spawn = w_range( 50, 50 );	// initial velocity
	w_range r_scale_spawn = w_range( 1, 1 );		// scale - spawn value and timeline
	w_range r_lifespan = w_range( 500, 1000 );		// lifetime in milliseconds
	w_range r_spin_per_sec = w_range( 0, 0 );		// ranges for controlling angle values
	w_range r_spin_spawn = w_range( 0, 0 );
	w_range r_dir_var = w_range( 0, 0 );			// variance for the spawn direction

	float s_max_spawn_per_sec = 0.0f;	// how many particles to spawn from this emitter, per second
	float a_dir = 0.0f;					// the base direction that particles start moving in when they spawn

	bool needs_warm_up = false;
	bool is_one_shot = false;

	a_emitter_params();
};

