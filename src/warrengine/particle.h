
namespace war
{

struct Particle final
{
	bool is_alive = false;
	Emitter_Parameters_Asset* params = nullptr;

	// storing this directly instead of using a w_transform
	// saves the size overhead of the virtual functions and extra floats
	Vec2 pos;

	// how long this particle should live, in ticks
	float life_span = 0.f;
	float life_span_save = 0.f;	// !! // saves the original life span for reference

	// angle of movement
	Vec2 v_dir = { 0, 0 };

	// world units to move, per second
	float velocity_per_sec = 0.f;

	// current particle state
	float base_scale = 0.f;

	float spin = 0.f;
	float spin_per_sec = 0.f;

	float anim_offset = 0.f;

	float gravity_accum = 0.f;
};

// make sure the particle struct doesn't get above 64 bytes
// for cache efficiency
//
// this is an attempt to keep the particles as small as possible in the
// memory pool. this should allow for more of them fitting into the
// cache and faster updating/rendering when there are thousands of them.

static_assert( sizeof( Particle ) <= 64 );

}
