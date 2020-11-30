#pragma once

// ----------------------------------------------------------------------------

struct w_particle_emitter : i_transform
{
	ec_emitter* parent_component = nullptr;

	a_emitter_params* params = nullptr;
	std::unique_ptr<w_particle_pool> particle_pool = nullptr;

	// how many particles from this emitter are allowed to
	// be alive at any given moment
	int max_particles_alive = 0;

	// floating point accumulator of time steps. whenever this is greater
	// than 1.0, it's time to spawn a particle.
	float particles_to_spawn_accum = 0.0f;

	void set_params( a_emitter_params* params );
	void update();
	void spawn_particle();
	void warm_up();
};
