#include "master_pch.h"
#include "master_header.h"

namespace war
{

transform* transform::set( vec2 pos, float angle, float scale )
{
	set_pos( pos );
	set_angle( angle );
	set_scale( scale );

	return this;
}

transform* transform::set_pos( vec2 pos )
{
	this->pos = pos;
	return this;
}

transform* transform::set_angle( float angle )
{
	this->angle = angle;
	return this;
}

transform* transform::set_scale( float scale )
{
	this->scale = glm::max( scale, 0.01f );
	return this;
}

transform* transform::add_pos( vec2 delta )
{
	return set_pos( this->pos + delta );
}

transform* transform::add_angle( float delta )
{
	return set_angle( this->angle + delta );
}

transform* transform::add_scale( float delta )
{
	return set_scale( this->scale + delta );
}

transform* transform::multiply_scale( float delta )
{
	return set_scale( this->scale * delta );
}

// ----------------------------------------------------------------------------
// transforms positions, not directions.

matrix transform::to_matrix() const
{
	matrix mtx;

	mtx.rotate( angle );
	mtx.scale( scale );
	mtx.translate( pos );

	return mtx;
}

// ----------------------------------------------------------------------------
// same as "to_matrix" except that the translation is ignored. this is for
// transforming directions, not positions.

matrix transform::to_matrix_vec() const
{
	matrix mtx;

	mtx.rotate( angle );
	mtx.scale( scale );

	return mtx;
}

}
