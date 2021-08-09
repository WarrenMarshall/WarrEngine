
#include "master_pch.h"
#include "master_header.h"

namespace war
{


std::vector<war::vec2> util_geo::generate_convex_shape( int sides, float radius )
{
	std::vector<vec2> verts;

	float angle = 0;
	float angle_step = 360.f / (float)( sides );

	for( auto x = 0 ; x < sides ; ++x )
	{
		verts.push_back( vec2::dir_from_angle( angle ) * radius );
		angle += angle_step;
	}

	return verts;
}

}