#pragma once

// ----------------------------------------------------------------------------

struct w_particle_spawner
{
	virtual void find_spawn_pos_for_new_particle( w_particle* particle, w_vec3& pos );
	virtual void parse_from_config_string( std::string value ) {}
};

struct w_particle_spawner_box : w_particle_spawner
{
	int w = 0, h = 0;

	w_particle_spawner_box() {}
	w_particle_spawner_box( const w_particle_spawner_box& other );
	w_particle_spawner_box( int w, int h );

	void find_spawn_pos_for_new_particle( w_particle* particle, w_vec3& pos );
	void parse_from_config_string( std::string value );
};



// audit any std::vector arrays with unique_ptr's ...
// emitter warm up stuff - can it be fixed?


struct w_particle_spawner_circle : w_particle_spawner
{
	float radius = 0;

	w_particle_spawner_circle() {}
	w_particle_spawner_circle( const w_particle_spawner_circle& other );
	w_particle_spawner_circle( float radius );

	void find_spawn_pos_for_new_particle( w_particle* particle, w_vec3& pos );
	void parse_from_config_string( std::string value );
};

// ----------------------------------------------------------------------------

struct w_particle_emitter : i_listener, i_transform
{
	a_emitter_params* params = nullptr;
	c_emitter* parent_component = nullptr;
	std::unique_ptr<w_particle_pool> particle_pool = nullptr;

	// the position of this particlesystem in the world. this is the origin
	// point for spawning particles.
	w_vec3 pos;

	// how many particles from this emitter are allowed to 
	// be alive at any given moment
	int max_particles_alive = 0;
	
	// floating point accumulator of time steps. whenever this is greater
	// than 1.0, it's time to spawn a particle.
	float particles_to_spawn_accum = 0.0f;

	w_particle_emitter();
	~w_particle_emitter();

	void on_listener_event_received( e_event_id event, void* object ) override;
	void set_params( a_emitter_params* params );
	virtual void post_spawn();
	void update();
	void spawn_particle();
	void warm_up();
};
