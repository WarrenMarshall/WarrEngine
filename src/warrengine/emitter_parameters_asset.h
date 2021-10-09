
namespace war
{

struct Emitter_Parameters_Asset final : Asset
{
	Texture_Asset* texture = nullptr;
	Timeline_Of_Values t_color = Timeline_Of_Values( e_timeline_type::color_type );
	Timeline_Of_Values t_alpha = Timeline_Of_Values( e_timeline_type::float_type );
	Timeline_Of_Values t_scale = Timeline_Of_Values( e_timeline_type::float_type );
	Particle_Spawner particle_spawner;

	Range r_velocity_spawn = Range( 50, 50 );	// initial velocity
	Range r_scale_spawn = Range( 1, 1 );		// scale - spawn value and timeline
	Range r_lifespan = Range( 500, 1000 );	// lifetime in milliseconds
	Range r_spin_per_sec = Range( 0, 0 );		// ranges for controlling angle values
	Range r_spin_spawn = Range( 0, 0 );
	Range r_dir_var = Range( 0, 0 );			// variance for the spawn direction

	float_t s_spawn_rate = 100.f;			// how many particles to spawn from this emitter, per second. aka the spawn rate.
	float_t a_dir = 0.f;					// the base direction that particles start moving in when they spawn
	float_t s_max_alive = 0.f;			// if > 0, dictates the maximum number of particles that can be alive at one time

	bool needs_warm_up : 1 = false;
	bool is_one_shot : 1 = false;
	bool uses_gravity : 1 = false;

	Emitter_Parameters_Asset();
};

}
