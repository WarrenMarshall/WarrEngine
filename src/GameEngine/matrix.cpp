#include "master_pch.h"
#include "master_header.h"

w_matrix w_matrix::make_identity()
{
	w_matrix mtx;
	mtx.m = glm::mat4( 1 );
	return mtx;
}

w_matrix* w_matrix::add_transform( const i_transform& t )
{
	translate( t.pos );
	rotate( t.angle );
	scale( t.scale );

	return this;
}

w_matrix* w_matrix::translate( const w_vec3& v )
{
	if( !fequals( v.x + v.y + v.z, 0.0f ) )
	{
		m = glm::translate( m, glm::vec3( v.x, v.y, v.z ) );
	}

	return this;
}

w_matrix* w_matrix::scale( float v )
{
	if( !fequals( v, 1.0f ) )
	{
		m = glm::scale( m, glm::vec3( v, v, v ) );
	}

	return this;
}

w_matrix* w_matrix::rotate( float v )
{
	if( fmod( v, 360.0f ) )
	{
		m = glm::rotate( m, glm::radians( v ), glm::vec3( 0.f, 0.f, 1.f ) );
	}

	return this;
}

