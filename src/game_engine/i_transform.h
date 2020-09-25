#pragma once

#include "datatypes.h"

struct i_transform
{
	// position
	w_vec2 pos;

	// rotation (around z axis)
	float angle_facing = 0.0f;

	// scale
	float scale = 1.0f;

	virtual void set_transform( const w_vec2& pos, const float angle, const float scale );
	virtual void set_position( const w_vec2& pos );
	virtual void set_angle( const float angle );
	virtual void set_scale( const float scale );
};
