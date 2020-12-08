#pragma once

// ----------------------------------------------------------------------------

struct e_paddle : w_entity
{
	virtual void phys_begin_contact( w_pending_collision& coll, w_entity* other ) override;
};

// ----------------------------------------------------------------------------

struct e_ball : w_entity
{
	static constexpr float radius = 5.f;
	static constexpr float speed = 35.f;

	virtual void phys_begin_contact( w_pending_collision& coll, w_entity* other ) override;
};

// ----------------------------------------------------------------------------
