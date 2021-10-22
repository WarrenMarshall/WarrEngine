
namespace war
{

// this is a static pool of particle objects that lives on the heap.
//
// the size is determined at construction and never changes.

struct Particle_Pool final : Object_Pool<Particle>
{
	Particle_Emitter* parent_emitter = nullptr;

	// the number of particles in the pool that are currently alive. this is
	// updated with each "update" cycle.
	i32 num_alive = 0;

	void draw();
	void update();
};

}
