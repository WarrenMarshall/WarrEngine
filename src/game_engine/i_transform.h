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

	void set_transform( const w_vec2& pos, const float& angle, const float& scale );
};
