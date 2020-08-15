#pragma once

// ----------------------------------------------------------------------------

struct GE_API w_particle_emitter : i_listener, i_transform
{
	a_emitter_params* params = nullptr;
	ec_emitter* parent_component = nullptr;
	std::unique_ptr<w_particle_pool> particle_pool = nullptr;

	// how many particles from this emitter are allowed to 
	// be alive at any given moment
	int max_particles_alive = 0;
	
	// floating point accumulator of time steps. whenever this is greater
	// than 1.0, it's time to spawn a particle.
	float particles_to_spawn_accum = 0.0f;

	~w_particle_emitter();

	void on_listener_event_received( e_event_id event, void* object ) override;
	void set_params( a_emitter_params* params );
	virtual void post_spawn();
	void update();
	void spawn_particle();
	void warm_up();
};
