
#include "master_pch.h"
#include "master_header.h"

w_matrix* w_matrix::add_transform( const i_transform& t )
{
	return add_transform( t.pos, t.angle, t.scale );
}

w_matrix* w_matrix::add_transform( const w_vec2& pos, const float angle, const float _scale )
{
	// save current matrix and reset internal to identity
	glm::mat4 save_m = m;
	m = glm::mat4( 1 );

	// apply requested transforms to the new matrix
	translate( pos );
	rotate( angle );
	scale( _scale );

	// multiply the new and old matrices together and make
	// the result current
	m = m * save_m;

	return this;
}

w_matrix* w_matrix::translate( const w_vec2& v )
{
	m = glm::translate( m, glm::vec3( v.x, v.y, 0.0f ) );
	return this;
}

w_matrix* w_matrix::scale( float v )
{
	if( !fequals( v, 1.0f ) )
	{
		m = glm::scale( m, glm::vec3( v, v, 1.0f ) );
	}

	return this;
}

w_matrix* w_matrix::scale( float x, float y )
{
	if( !fequals( x, 1.0f ) || !fequals( y, 1.0f ) )
	{
		m = glm::scale( m, glm::vec3( x, y, 1.0f ) );
	}

	return this;
}

w_matrix* w_matrix::rotate( float v )
{
	if( !fequals( v, 0.0f ) )
	{
		m = glm::rotate( m, glm::radians( v ), glm::vec3( 0.f, 0.f, 1.f ) );
	}

	return this;
}

w_vec2 w_matrix::transform_vec2( const w_vec2& vtx )
{
	auto v = m * glm::vec4( vtx.x, vtx.y, 0.0f, 1.0f );
	return w_vec2( v.x, v.y );
}
