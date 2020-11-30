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

	void it_set( w_vec2 pos, float angle, float scale );
	void it_set_position( w_vec2 pos );
	void it_set_angle( float angle );
	void it_set_scale( float scale );
};
