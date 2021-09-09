
namespace war
{

struct Emitter_Parameters_Asset : Asset
{
	Texture_Asset* texture = nullptr;
	Timeline_Values t_color = Timeline_Values( e_timeline_type::color_type );
	Timeline_Values t_alpha = Timeline_Values( e_timeline_type::float_type );
	Timeline_Values t_scale = Timeline_Values( e_timeline_type::float_type );
	Particle_Spawner particle_spawner;

	Range<float_t> r_velocity_spawn = Range<float_t>( 50, 50 );	// initial velocity
	Range<float_t> r_scale_spawn = Range<float_t>( 1, 1 );		// scale - spawn value and timeline
	Range<float_t> r_lifespan = Range<float_t>( 500, 1000 );	// lifetime in milliseconds
	Range<float_t> r_spin_per_sec = Range<float_t>( 0, 0 );		// ranges for controlling angle values
	Range<float_t> r_spin_spawn = Range<float_t>( 0, 0 );
	Range<float_t> r_dir_var = Range<float_t>( 0, 0 );			// variance for the spawn direction

	float_t s_spawn_rate = 100.f;			// how many particles to spawn from this emitter, per second. aka the spawn rate.
	float_t a_dir = 0.f;					// the base direction that particles start moving in when they spawn
	float_t s_max_alive = 0.f;			// if > 0, dictates the maximum number of particles that can be alive at one time

	bool needs_warm_up = false;
	bool is_one_shot = false;
	bool uses_gravity = false;

	Emitter_Parameters_Asset();
};

}
