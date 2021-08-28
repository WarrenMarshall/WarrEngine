#include "master_pch.h"
#include "master_header.h"

namespace war
{

Transform* Transform::set( Vec2 pos, float angle, float scale )
{
	set_pos( pos );
	set_angle( angle );
	set_scale( scale );

	return this;
}

Transform* Transform::set_pos( Vec2 pos )
{
	this->pos = pos;
	return this;
}

Transform* Transform::set_angle( float angle )
{
	this->angle = angle;
	return this;
}

Transform* Transform::set_scale( float scale )
{
	this->scale = glm::max( scale, 0.01f );
	return this;
}

Transform* Transform::add_pos( Vec2 delta )
{
	return set_pos( this->pos + delta );
}

Transform* Transform::add_angle( float delta )
{
	return set_angle( this->angle + delta );
}

Transform* Transform::add_scale( float delta )
{
	return set_scale( this->scale + delta );
}

Transform* Transform::multiply_scale( float delta )
{
	return set_scale( this->scale * delta );
}

// ----------------------------------------------------------------------------
// transforms positions, not directions.

Matrix Transform::to_matrix() const
{
	Matrix mtx;

	mtx.rotate( angle );
	mtx.scale( scale );
	mtx.translate( pos );

	return mtx;
}

// ----------------------------------------------------------------------------
// same as "to_matrix" except that the translation is ignored. this is for
// transforming directions, not positions.

Matrix Transform::to_matrix_vec() const
{
	Matrix mtx;

	mtx.rotate( angle );
	mtx.scale( scale );

	return mtx;
}

}
