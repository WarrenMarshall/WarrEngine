#pragma once

struct e_player : w_entity
{
	virtual void collided_with( ec_collider* collider, w_entity* entity_hit, c2Manifold& hit ) final;
};

