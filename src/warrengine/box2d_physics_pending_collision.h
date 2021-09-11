
namespace war::box2d_physics
{

struct Pending_Collision final
{
	Entity* entity_a = nullptr;
	b2Fixture* fixture_a = nullptr;

	Entity* entity_b = nullptr;
	b2Fixture* fixture_b = nullptr;

	b2Manifold manifold = {};
	b2WorldManifold wmanifold = {};
};

}
