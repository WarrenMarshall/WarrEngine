#include "master_pch.h"
#include "master_header.h"

w_matrix* w_matrix::add_transform( const i_transform& t )
{
	translate( t.pos );
	scale( t.scale );
	rotate( t.angle_facing );

	return this;
}

w_matrix* w_matrix::add_transform( const w_vec2& pos, const float angle, const float _scale )
{
	translate( pos );
	scale( _scale );
	rotate( angle );

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
		m = glm::scale( m, glm::vec3( v, v, v ) );
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

void w_matrix::transform_vec2( w_vec2& vtx )
{
	glm::vec4 v4( vtx.x, vtx.y, 0.0f, 1.0f );
	v4 = m * v4;

	vtx.x = v4.x;
	vtx.y = v4.y;
}

void w_matrix::transform_vec3( w_vec3& vtx )
{
	glm::vec4 v4( vtx.x, vtx.y, vtx.z, 1.0f );
	v4 = m * v4;

	vtx.x = v4.x;
	vtx.y = v4.y;
	vtx.z = v4.z;
}
