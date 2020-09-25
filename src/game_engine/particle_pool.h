#pragma once

/*
	this is a static pool of particle objects that lives on the heap.

	the size is determined at construction and never changes.

*/
struct w_particle_pool
{
	w_particle_emitter* parent_emitter = nullptr;

	// particle object pool
	std::unique_ptr<w_particle[]> particles = nullptr;
	int pool_sz = -1;

	// which particle we're going to use next. this wraps around the vector
	// above, endlessly, re-using particles.
	int idx = 0;

	// the number of particles in the pool that are currently alive.
	int num_alive = 0;

	w_particle_pool( int num_particles );

	w_particle* get_next_particle();
	void draw();
	void update();
};
