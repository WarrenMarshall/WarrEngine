
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Bounding_Box::Bounding_Box()
{
	reset();
}

void Bounding_Box::add( const Vec2& vtx )
{
	min.x = glm::min( min.x, vtx.x );
	min.y = glm::min( min.y, vtx.y );

	max.x = glm::max( max.x, vtx.x );
	max.y = glm::max( max.y, vtx.y );
}

void Bounding_Box::reset()
{
	min.x = min.y = std::numeric_limits<float>::max();
	max.x = max.y = std::numeric_limits<float>::min();
}

Bounding_Box Bounding_Box::operator+( const Vec2& v ) const
{
	Bounding_Box bb;
	bb.add( this->min );
	bb.add( this->max );
	bb.add( v );

	return bb;
}

Bounding_Box Bounding_Box::operator+=( const Vec2& v )
{
	*this = *this + v;
	return *this;
}

Bounding_Box Bounding_Box::operator+( const Bounding_Box& bbox ) const
{
	Bounding_Box bb;
	bb = *this;
	bb.add( bbox.min );
	bb.add( bbox.max );

	return bb;
}

Bounding_Box Bounding_Box::operator+=( const Bounding_Box& bbox )
{
	*this = *this + bbox;
	return *this;
}

}
