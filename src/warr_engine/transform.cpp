#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_transform::w_transform()
{
	matrix.set_identity();
	matrix_dir.set_identity();
}

w_transform* w_transform::set( w_vec2 pos, float angle, float scale )
{
	set_pos( pos );
	set_angle( angle );
	set_scale( scale );

	return this;
}

w_transform* w_transform::set_pos( w_vec2 pos )
{
	this->pos.x = snap_to_pixel( pos.x );
	this->pos.y = snap_to_pixel( pos.y );

	rebuild_matrix();

	return this;
}

w_transform* w_transform::set_angle( float angle )
{
	this->angle = snap_to_pixel( angle );

	rebuild_matrix();

	return this;
}

w_transform* w_transform::set_scale( float scale )
{
	this->scale = scale;

	rebuild_matrix();

	return this;
}

w_transform* w_transform::add_pos_delta( w_vec2 delta )
{
	return set_pos( this->pos + delta );
}

w_transform* w_transform::add_angle_delta( float delta )
{
	return set_angle( this->angle + delta );
}

w_transform* w_transform::add_scale_delta( float delta )
{
	return set_scale( this->scale + delta );
}

// compiles the current transform into a matrix for ease of use

void w_transform::rebuild_matrix()
{
	matrix.set_identity();
	matrix.translate( pos );
	matrix.rotate( angle );
	matrix.scale( scale );

	matrix_dir.set_identity();
	matrix_dir.rotate( angle );
}

// "pos" represents a position, not a direction

w_vec2 w_transform::transform( const w_vec2& pos )
{
	return matrix.transform( pos );
}

w_vec2 w_transform::inverse_transform( const w_vec2& pos )
{
	return matrix.inverse_transform( pos );
}
