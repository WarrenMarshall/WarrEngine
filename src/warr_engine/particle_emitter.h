
namespace war
{

struct particle_emitter
{
	emitter_component* parent_component = nullptr;

	emitter_parameters_asset* params = nullptr;
	particle_pool pool;

	// how many particles from this emitter are allowed to
	// be alive at any given moment
	int max_particles_alive = 0;

	// floating point accumulator of time steps. whenever this is greater
	// than 1.0, it's time to spawn a particle.
	float particles_to_spawn_accum = 0.f;

	void set_params( emitter_parameters_asset* params );
	void update();
	void spawn_particle();
	void warm_up();
};

}
