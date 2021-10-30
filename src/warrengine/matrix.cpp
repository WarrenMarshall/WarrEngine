
#include "master_pch.h"

namespace war
{

Matrix::Matrix()
{
	identity();
}

Matrix::Matrix( glm::mat4 glm_mtx )
	: m( glm_mtx )
{
}

void Matrix::identity()
{
	m = glm::mat4( 1 );
}

void Matrix::apply_transform( const Transform& transform )
{
	apply_transform( transform.pos, transform.angle, { transform.scale, transform.scale } );
}

void Matrix::apply_transform( const Vec2& pos, f32 angle, f32 scale )
{
	apply_transform( pos, angle, { scale, scale } );
}

void Matrix::apply_transform( const Vec2& pos, f32 angle, const Vec2& scale )
{
	// apply requested transforms to this matrix
	//
	// NOTE : read from bottom to top:
	//
	// - scale around origin
	// - rotate around origin
	// - translate to position
	//
	// scale and rotation are interchangeable, but translate needs to be last (err, top)

	this->translate( pos );
	this->rotate( angle );
	this->scale( scale );
}

void Matrix::translate( const Vec2& v )
{
	m = glm::translate( m, glm::vec3( v.x, v.y, 0.f ) );
}

void Matrix::scale( f32 v )
{
	m = glm::scale( m, glm::vec3( v, v, 1.f ) );
}

void Matrix::scale( const Vec2& v )
{
	m = glm::scale( m, glm::vec3( v.x, v.y, 1.f ) );
}

void Matrix::rotate( f32 v )
{
	m = glm::rotate( m, glm::radians( v ), glm::vec3( 0.f, 0.f, 1.f ) );
}

void Matrix::invert()
{
	m = glm::inverse( m );
}

// ----------------------------------------------------------------------------

Vec2 Matrix::transform_vec2( const Vec2& v ) const
{
	auto new_v = m * glm::vec4( v.x, v.y, 0.f, 1.f );
	return Vec2( new_v.x, new_v.y );
}

void Matrix::transform_vec2( Vec2* v ) const
{
	auto new_v = m * glm::vec4( v->x, v->y, 0.f, 1.f );
	*v = Vec2( new_v.x, new_v.y );
}

// ----------------------------------------------------------------------------

Matrix Matrix::operator*( Matrix& mtx ) const
{
	glm::mat4 glm_mtx = this->m * mtx.m;
	return Matrix( glm_mtx );
}

Matrix Matrix::operator*=( Matrix& mtx )
{
	m *= mtx.m;
	return *this;
}

}
