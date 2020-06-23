#pragma once

#include "datatypes.h"

struct i_transform
{
	// position
	w_vec3 pos;

	// rotation (around z axis)
	float angle = 0.0f;

	// scale
	float scale = 1.0f;

	i_transform();
	~i_transform();

	void set_transform( const w_vec3& pos, const float& angle, const float& scale );
};
