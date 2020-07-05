#pragma once

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

struct w_particle_spawner_circle : w_particle_spawner
{
	float radius = 0;

	w_particle_spawner_circle() {}
	w_particle_spawner_circle( const w_particle_spawner_circle& other );
	w_particle_spawner_circle( float radius );

	void find_spawn_pos_for_new_particle( w_particle* particle, w_vec3& pos );
	void parse_from_config_string( std::string value );
};
