#pragma once

#include "datatypes.h"

struct i_transform
{
	virtual ~i_transform() = default;

	// position
	w_vec2 pos;

	// rotation (around z axis)
	float angle = 0.0f;

	// scale
	float scale = 1.0f;

	virtual void set_transform( w_vec2 pos, float angle, float scale );
	virtual void set_position( w_vec2 pos );
	virtual void set_angle( float angle );
	virtual void set_scale( float scale );
};
