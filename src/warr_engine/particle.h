#pragma once

// this is an attempt to keep the particles as small as possible in the
// memory pool. this should allow for more of them fitting into the
// cache and faster updating/rendering when there are thousands of them.

struct w_particle final
{
	a_texture* tex = nullptr;
	a_emitter_params* params = nullptr;

	// storing this directly instead of deriving from i_transform
	// saves the size overhead of the virtual functions and extra floats
	w_vec2 pos;

	// how long this particle should live, in ticks
	float life_span = 0.0f;
	float life_span_save = 0.0f;	// saves the original life span for reference

	// angle of movement
	float a_dir = 0.0f;	// #todo : I think this is only used for spawning the particle - can we factor it away?
	w_vec2 v_dir = { 0,0 };

	// world units to move, per second
	float velocity_per_sec = 0.0f;

	// current particle state
	float base_scale = 0.0f;

	float spin = 0.0f;
	float spin_per_sec = 0.0f;

	void update();
	_NODISCARD constexpr bool is_alive();
};

// make sure the particle struct doesn't get above 64 bytes
// for cache efficiency
static_assert( sizeof( w_particle ) <= 64 );
