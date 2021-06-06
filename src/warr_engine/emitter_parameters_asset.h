
namespace war
{

struct emitter_parameters_asset final : asset
{
	texture_asset* texture = nullptr;
	timeline t_color = timeline( timeline_type::color_type );
	timeline t_alpha = timeline( timeline_type::float_type );
	timeline t_scale = timeline( timeline_type::float_type );
	particle_spawner particle_spawner;

	range<float> r_velocity_spawn = range<float>( 50, 50 );	// initial velocity
	range<float> r_scale_spawn = range<float>( 1, 1 );		// scale - spawn value and timeline
	range<float> r_lifespan = range<float>( 500, 1000 );	// lifetime in milliseconds
	range<float> r_spin_per_sec = range<float>( 0, 0 );		// ranges for controlling angle values
	range<float> r_spin_spawn = range<float>( 0, 0 );
	range<float> r_dir_var = range<float>( 0, 0 );			// variance for the spawn direction

	float s_spawn_rate = 100.f;			// how many particles to spawn from this emitter, per second. aka the spawn rate.
	float a_dir = 0.f;					// the base direction that particles start moving in when they spawn
	float s_max_alive = 0.f;			// if > 0, dictates the maximum number of particles that can be alive at one time

	bool needs_warm_up = false;
	bool is_one_shot = false;
	bool uses_gravity = false;

	emitter_parameters_asset();
};

}
