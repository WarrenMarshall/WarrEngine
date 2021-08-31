
namespace war
{

struct Emitter_Parameters_Asset : Asset
{
	Texture_Asset* texture = nullptr;
	Timeline_Values t_color = Timeline_Values( e_timeline_type::color_type );
	Timeline_Values t_alpha = Timeline_Values( e_timeline_type::float_type );
	Timeline_Values t_scale = Timeline_Values( e_timeline_type::float_type );
	Particle_Spawner particle_spawner;

	Range<float> r_velocity_spawn = Range<float>( 50, 50 );	// initial velocity
	Range<float> r_scale_spawn = Range<float>( 1, 1 );		// scale - spawn value and timeline
	Range<float> r_lifespan = Range<float>( 500, 1000 );	// lifetime in milliseconds
	Range<float> r_spin_per_sec = Range<float>( 0, 0 );		// ranges for controlling angle values
	Range<float> r_spin_spawn = Range<float>( 0, 0 );
	Range<float> r_dir_var = Range<float>( 0, 0 );			// variance for the spawn direction

	float s_spawn_rate = 100.f;			// how many particles to spawn from this emitter, per second. aka the spawn rate.
	float a_dir = 0.f;					// the base direction that particles start moving in when they spawn
	float s_max_alive = 0.f;			// if > 0, dictates the maximum number of particles that can be alive at one time

	bool needs_warm_up = false;
	bool is_one_shot = false;
	bool uses_gravity = false;

	Emitter_Parameters_Asset();
};

}
