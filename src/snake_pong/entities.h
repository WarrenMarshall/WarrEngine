#pragma once

// ----------------------------------------------------------------------------

struct e_paddle : w_entity
{
	virtual void on_collision_begin( w_pending_collision& coll, w_entity* other ) override;
};

// ----------------------------------------------------------------------------

struct e_ball : w_entity
{
	static constexpr float radius = 6.f;
	static constexpr float speed = 35.f;

	e_ball();

	virtual void on_collision_begin( w_pending_collision& coll, w_entity* other ) override;
};

// ----------------------------------------------------------------------------
