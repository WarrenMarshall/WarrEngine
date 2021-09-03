
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const Rect Rect::zero = Rect( 0, 0, 0, 0 );

Rect::Rect( float x, float y )
	: x( x ), y( y )
{
}

Rect::Rect( float x, float y, float w, float h )
	: x( x ), y( y ), w( w ), h( h )
{
}

Rect::Rect( int32_t x, int32_t y, int32_t w, int32_t h )
	: x( (float)x ), y( (float)y ), w( (float)w ), h( (float)h )
{
}

Rect::Rect( const Vec2& top_left, const Vec2& bottom_right )
	: x( top_left.x ), y( top_left.y ), w( bottom_right.x - top_left.x ), h( bottom_right.y - top_left.y )
{
}

Vec2 Rect::extents() const
{
	return { ( w - x ), ( h - y ) };
}

Vec2 Rect::top_left() const
{
	return { x, y };
}

Vec2 Rect::bottom_right() const
{
	return { x + w, y + h };
}

Vec2 Rect::get_midpoint() const
{
	return Vec2( x + ( w / 2.f ), y + ( h / 2.f ) );
}

// returns a position within this rectangle that represents
// the best spot to start drawing from, given the alignment
// requested.

Vec2 Rect::get_pos_from_alignment( e_align_t align ) const
{
	Vec2 pos = { x, y };

	auto mid = get_midpoint();
	auto br = bottom_right();

	if( align & e_align::right )
	{
		pos.x = br.x;
	}
	if( align & e_align::hcenter )
	{
		pos.x = mid.x;
	}

	if( align & e_align::bottom )
	{
		pos.y = br.y;
	}
	if( align & e_align::vcenter )
	{
		pos.y = mid.y;
	}

	return pos;
}

// inflates/deflates a rectangle by "val". this affects all 4 sides.

Rect Rect::grow( float val )
{
	x -= val;
	y -= val;
	w += val * 2.f;
	h += val * 2.f;

	return *this;
}

Rect Rect::shrink( float val )
{
	x += val;
	y += val;
	w -= val * 2.f;
	h -= val * 2.f;

	return *this;
}

// is "pos" within the bounds of this rectangle?

bool Rect::contains_point( Vec2 pos )
{
	return c2AABBtoPoint( to_c2AABB(), pos.to_c2v() );
}

// NOTE : passing in a sz of < 1.f is translated as passing in a percentage of
// the existing width or height.

Rect Rect::cut_left( float sz )
{
	if( sz < 1.f )
	{
		sz = w * sz;
	}

	Rect result = { x, y, sz, h };

	x += sz;
	w -= sz;

	return result;
}

Rect Rect::cut_right( float sz )
{
	if( sz < 1.f )
	{
		sz = w * sz;
	}

	Rect result = { x + ( w - sz ), y, sz, h };

	w -= sz;

	return result;
}

Rect Rect::cut_top( float sz )
{
	if( sz < 1.f )
	{
		sz = h * sz;
	}

	Rect result = { x, y, w, sz };

	y += sz;
	h -= sz;

	return result;
}

Rect Rect::cut_bottom( float sz )
{
	if( sz < 1.f )
	{
		sz = h * sz;
	}

	Rect result = { x, y + ( h - sz ), w, sz };

	h -= sz;

	return result;
}

// returns whatever is remaining and returns it whole

Rect Rect::cut()
{
	Rect result = *this;

	*this = Rect::zero;

	return result;
}

bool Rect::operator==( const Rect& rhs ) const
{
	return( fequals( rhs.x, x ) and fequals( rhs.y, y ) and fequals( rhs.w, w ) and fequals( rhs.h, h ) );
}

Rect Rect::operator+( const Vec2& v ) const
{
	return Rect( this->x + v.x, this->y + v.y, this->w, this->h );
}

Rect Rect::operator+=( const Vec2& v )
{
	*this = *this + v;
	return *this;
}

Rect Rect::operator-( const Vec2& v ) const
{
	return Rect( this->x - v.x, this->y - v.y, this->w, this->h );
}

Rect Rect::operator-=( const Vec2& v )
{
	*this = *this - v;
	return *this;
}

Rect Rect::operator+( const Rect& rhs ) const
{
	return Rect( x + rhs.x, y + rhs.y, w + rhs.w, h + rhs.h );
}

Rect Rect::operator-( const Rect& rhs ) const
{
	return Rect( x - rhs.x, y - rhs.y, w - rhs.w, h - rhs.h );
}

Rect Rect::operator*( float v ) const
{
	return Rect( this->x * v, this->y * v, this->w * v, this->h * v );
}

Rect Rect::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

c2AABB Rect::to_c2AABB() const
{
	c2AABB aabb = {};

	aabb.min.x = to_simple( x );
	aabb.min.y = to_simple( y );
	aabb.max.x = to_simple( x + w );
	aabb.max.y = to_simple( y + h  );

	return aabb;
}

Rect Rect::create_centered( float sz )
{
	return Rect::create_centered( sz, sz );
}

Rect Rect::create_centered( float w, float h )
{
	auto hw = w / 2.f;
	auto hh = h / 2.f;

	return Rect( -hw, -hh, w, h );
}

}
