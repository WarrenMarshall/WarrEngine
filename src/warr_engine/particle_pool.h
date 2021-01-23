#pragma once

// this is a static pool of particle objects that lives on the heap.
//
// the size is determined at construction and never changes.

struct w_particle_pool
{
	w_particle_emitter* parent_emitter = nullptr;

	// particle object pool
	std::vector<w_particle> particles;	// preallocates the memory for the particle pool
	w_particle* data_ptr = nullptr;		// direct pointer to the vector memory
	int pool_sz = -1;					// the total number of particle slots in this pool

	// which particle we're going to use next. this wraps around the vector
	// above, endlessly, re-using particles.
	int idx = 0;

	// the number of particles in the pool that are currently alive. this is
	// updated with each "update" cycle.
	int num_alive = 0;

	w_particle_pool( int num_particles );

	[[nodiscard]] w_particle* get_next_particle();
	void draw();
	void update();
};
