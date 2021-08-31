
namespace war
{

struct Particle_Spawner
{
	e_particle_spawner_type_t type = e_particle_spawner_type::point;

	union
	{
		struct
		{
			// for e_particle_spawner_type::box
			int w, h;
		};
		struct
		{
			// for e_particle_spawner_type::circle
			float radius;
		};
	};

	void find_spawn_pos_for_new_particle( Particle* particle );
	void parse_from_config_string( std::string_view value );
};

}
