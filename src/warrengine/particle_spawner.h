
namespace war
{

struct particle_spawner final
{
	e_particle_spawner_type type = particle_spawner_type::point;

	union
	{
		struct
		{
			// for particle_spawner_type::box
			int w, h;
		};
		struct
		{
			// for particle_spawner_type::circle
			float radius;
		};
	};

	void find_spawn_pos_for_new_particle( particle* particle );
	void parse_from_config_string( std::string_view value );
};

}
