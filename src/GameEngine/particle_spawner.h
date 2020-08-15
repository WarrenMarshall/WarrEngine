#pragma once

struct GE_API w_particle_spawner
{
	virtual ~w_particle_spawner() = default;

	virtual void find_spawn_pos_for_new_particle( w_particle* particle, w_vec2& pos );
	virtual void parse_from_config_string( std::string_view value ) {}
};

struct GE_API w_particle_spawner_box : w_particle_spawner
{
	int w = 0, h = 0;

	w_particle_spawner_box() = default;
	w_particle_spawner_box( const w_particle_spawner_box& other );
	w_particle_spawner_box( int w, int h );

	void find_spawn_pos_for_new_particle( w_particle* particle, w_vec2& pos ) override;
	void parse_from_config_string( std::string_view value ) override;
};

struct GE_API w_particle_spawner_circle : w_particle_spawner
{
	float radius = 0;

	w_particle_spawner_circle() = default;
	w_particle_spawner_circle( const w_particle_spawner_circle& other );
	w_particle_spawner_circle( float radius );

	void find_spawn_pos_for_new_particle( w_particle* particle, w_vec2& pos ) override;
	void parse_from_config_string( std::string_view value ) override;
};
