
#include "master_pch.h"
#include "master_header.h"

namespace war
{

matrix::matrix()
{
	identity();
}

matrix::matrix( glm::mat4 glm_mtx )
	: m( glm_mtx )
{
}

void matrix::identity()
{
	m = glm::mat4( 1 );
}

void matrix::apply_transform( const transform& transform )
{
	apply_transform( transform.pos, transform.angle, { transform.scale, transform.scale } );
}

void matrix::apply_transform( const vec2& pos, float angle, float scale )
{
	apply_transform( pos, angle, { scale, scale } );
}

void matrix::apply_transform( const vec2& pos, float angle, const vec2& scale )
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

void matrix::translate( const vec2& v )
{
	m = glm::translate( m, glm::vec3( v.x, v.y, 0.f ) );
}

void matrix::scale( float v )
{
	m = glm::scale( m, glm::vec3( v, v, 1.f ) );
}

void matrix::scale( const vec2& v )
{
	m = glm::scale( m, glm::vec3( v.x, v.y, 1.f ) );
}

void matrix::rotate( float v )
{
	m = glm::rotate( m, glm::radians( v ), glm::vec3( 0.f, 0.f, 1.f ) );
}

void matrix::invert()
{
	m = glm::inverse( m );
}

// ----------------------------------------------------------------------------

vec2 matrix::transform_vec2( const vec2& v ) const
{
	auto new_v = m * glm::vec4( v.x, v.y, 0.f, 1.f );
	return vec2( new_v.x, new_v.y );
}

// ----------------------------------------------------------------------------

matrix matrix::operator*( matrix& mtx ) const
{
	glm::mat4 glm_mtx = this->m * mtx.m;
	return matrix( glm_mtx );
}

matrix matrix::operator*=( matrix& mtx )
{
	m *= mtx.m;
	return *this;
}

}
