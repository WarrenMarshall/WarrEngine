#include "master_pch.h"
#include "master_header.h"

w_matrix* w_matrix::add_transform( const i_transform& t )
{
	translate( t.pos );
	rotate( t.angle );
	scale( t.scale );

	return this;
}

w_matrix* w_matrix::add_transform( const w_vec3& pos, const float& angle, const float& _scale )
{
	translate( pos );
	rotate( angle );
	scale( _scale );

	return this;
}

w_matrix* w_matrix::translate( const w_vec3& v )
{
	m = glm::translate( m, glm::vec3( v.x, v.y, v.z ) );
	return this;
}

w_matrix* w_matrix::scale( float v )
{
	m = glm::scale( m, glm::vec3( v, v, v ) );
	return this;
}

w_matrix* w_matrix::rotate( float v )
{
	m = glm::rotate( m, glm::radians( v ), glm::vec3( 0.f, 0.f, 1.f ) );
	return this;
}
