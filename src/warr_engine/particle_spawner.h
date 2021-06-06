
namespace war
{

struct particle_spawner
{
	e_particle_spawner_type type = particle_spawner_type::point;

	// for particle_spawner_type::box
	int w = 0, h = 0;

	// for particle_spawner_type::circle
	float radius = 0.f;

	void find_spawn_pos_for_new_particle( particle* particle );
	void parse_from_config_string( std::string_view value );
};

}
