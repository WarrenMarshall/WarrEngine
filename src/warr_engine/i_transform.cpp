
#include "master_pch.h"
#include "master_header.h"

void i_transform::it_set( w_vec2 pos, float angle, float scale )
{
	it_set_position( pos );
	it_set_angle( angle );
	it_set_scale( scale );
}

void i_transform::it_set_position( w_vec2 pos )
{
	this->pos.x = snap_to_pixel( pos.x );
	this->pos.y = snap_to_pixel( pos.y );
}

void i_transform::it_set_angle( float angle )
{
	this->angle = snap_to_pixel( angle );
}

void i_transform::it_set_scale( float scale )
{
	this->scale = scale;
}
