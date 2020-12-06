#pragma once

struct e_ball : w_entity
{
	virtual void phys_begin_contact( const w_physics_responder& responder, const char* other_tag, const w_entity* other ) override;
};
