
#include "master_pch.h"
#include "master_header.h"

void i_transform::set_transform( const w_vec2& pos, const float angle, const float scale )
{
	this->pos = pos;
	this->angle_facing = angle;
	this->scale = scale;
}

void i_transform::set_position( const w_vec2& pos )
{
	this->pos = pos;
}

void i_transform::set_angle( const float angle )
{
	this->angle_facing = angle;
}

void i_transform::set_scale( const float scale )
{
	this->scale = scale;
}
