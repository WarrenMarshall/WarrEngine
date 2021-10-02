
#include "master_pch.h"
#include "master_header.h"

namespace war
{

Bounding_Box::Bounding_Box()
{
	reset();
}

Bounding_Box::Bounding_Box( const Vec2& v1, const Vec2& v2 )
{
	reset();
	add( v1 );
	add( v2 );
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
	min.x = min.y = 999999.f;
	max.x = max.y = -999999.f;
}

Vec2 Bounding_Box::get_random_spot() const
{
	auto dist_x = ( Vec2( max.x, 0.f ) - Vec2( min.x, 0.f ) ).get_size();
	auto dist_y = ( Vec2( 0.f, max.y ) - Vec2( 0.f, min.y ) ).get_size();

	return Vec2( min.x + ( dist_x * Random::getf() ), min.y + ( dist_y * Random::getf() ) );
}

Rect Bounding_Box::as_rect() const
{
	return Rect( min.x, max.y, max.x - min.x, max.y - min.y );
}

Bounding_Box Bounding_Box::operator+( const Vec2& v ) const
{
	Bounding_Box bb = *this;
	bb.add( v );

	return bb;
}

Bounding_Box Bounding_Box::operator+=( const Vec2& v )
{
	*this = *this + v;
	return *this;
}

Bounding_Box Bounding_Box::operator+( const Rect& r ) const
{
	Bounding_Box bb = *this;
	bb.add( { r.x, r.y } );
	// warren
	//bb.add( { r.x + r.w, r.y - r.h } );
	bb.add( { r.x + r.w, r.y + r.h } );

	return bb;
}

Bounding_Box Bounding_Box::operator+=( const Rect& r )
{
	*this = *this + r;
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
