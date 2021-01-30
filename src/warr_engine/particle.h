#pragma once

struct w_particle final
{
	bool is_alive = true;
	a_emitter_params* params = nullptr;

	// storing this directly instead of using a w_transform
	// saves the size overhead of the virtual functions and extra floats
	w_vec2 pos;

	// how long this particle should live, in ticks
	float life_span = 0.0f;
	float life_span_save = 0.0f;	// saves the original life span for reference

	// angle of movement
	w_vec2 v_dir = { 0,0 };

	// world units to move, per second
	float velocity_per_sec = 0.0f;

	// current particle state
	float base_scale = 0.0f;

	float spin = 0.0f;
	float spin_per_sec = 0.0f;

	void update();
};

// make sure the particle struct doesn't get above 64 bytes
// for cache efficiency
//
// this is an attempt to keep the particles as small as possible in the
// memory pool. this should allow for more of them fitting into the
// cache and faster updating/rendering when there are thousands of them.

static_assert( sizeof( w_particle ) <= 64 );
