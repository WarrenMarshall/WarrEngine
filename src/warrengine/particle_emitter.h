
namespace war
{

struct Particle_Emitter
{
	Emitter_Component* parent_component = nullptr;

	Emitter_Parameters_Asset* params = nullptr;
	Particle_Pool pool;

	// how many particles from this emitter are allowed to
	// be alive at any given moment
	int32_t max_particles_alive = 0;

	// floating point accumulator of time steps. whenever this is greater
	// than 1.0, it's time to spawn a particle.
	float_t particles_to_spawn_accum = 0.f;

	void set_params( Emitter_Parameters_Asset* params );
	void update();
	void spawn_particle();
	void warm_up();
};

}
