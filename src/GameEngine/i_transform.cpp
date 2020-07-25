#include "master_pch.h"
#include "master_header.h"

void i_transform::set_transform( const w_vec2& pos, const float& angle, const float& scale )
{
	this->pos = pos;
	this->angle = angle;
	this->scale = scale;
}
