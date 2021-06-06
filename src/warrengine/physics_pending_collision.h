
namespace war
{

struct physics_pending_collision
{
	entity* entity_a = nullptr;
	b2Fixture* fixture_a = nullptr;
	entity* entity_b = nullptr;
	b2Fixture* fixture_b = nullptr;
	b2Manifold manifold = {};
	b2WorldManifold wmanifold = {};
};

}
