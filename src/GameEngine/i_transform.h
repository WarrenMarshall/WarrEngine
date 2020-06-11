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

	inline void set_transform( const w_vec3& pos, float angle, float scale )
	{
		this->pos = pos;
		this->angle = angle;
		this->scale = scale;
	}
};
