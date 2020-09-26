
#include "master_pch.h"
#include "master_header.h"

void i_transform::set_transform( w_vec2 pos, float angle, float scale )
{
	set_position( pos );
	set_angle( angle );
	set_scale( scale );
}

void i_transform::set_position( w_vec2 pos )
{
	this->pos = pos;
}

void i_transform::set_angle( float angle )
{
	this->angle_facing = angle;
}

void i_transform::set_scale( float scale )
{
	this->scale = scale;
}
