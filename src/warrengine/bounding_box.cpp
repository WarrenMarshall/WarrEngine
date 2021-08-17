
#include "master_pch.h"
#include "master_header.h"

namespace war
{

bounding_box::bounding_box()
{
	reset();
}

void bounding_box::add( const vec2& vtx )
{
	min.x = glm::min( min.x, vtx.x );
	min.y = glm::min( min.y, vtx.y );

	max.x = glm::max( max.x, vtx.x );
	max.y = glm::max( max.y, vtx.y );
}

void bounding_box::reset()
{
	min.x = min.y = std::numeric_limits<float>::max();
	max.x = max.y = std::numeric_limits<float>::min();
}

bounding_box bounding_box::operator+( const vec2& v ) const
{
	bounding_box bb;
	bb.add( this->min );
	bb.add( this->max );
	bb.add( v );

	return bb;
}

bounding_box bounding_box::operator+=( const vec2& v )
{
	*this = *this + v;
	return *this;
}

bounding_box bounding_box::operator+( const bounding_box& bbox ) const
{
	bounding_box bb;
	bb = *this;
	bb.add( bbox.min );
	bb.add( bbox.max );

	return bb;
}

bounding_box bounding_box::operator+=( const bounding_box& bbox )
{
	*this = *this + bbox;
	return *this;
}

}
