
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const rect rect::zero = rect( 0, 0, 0, 0 );

rect::rect( float x, float y )
	: x( x ), y( y )
{
}

rect::rect( float x, float y, float w, float h )
	: x( x ), y( y ), w( w ), h( h )
{
}

rect::rect( int x, int y, int w, int h )
	: x( (float)x ), y( (float)y ), w( (float)w ), h( (float)h )
{
}

rect::rect( int16 x, int16 y, int16 w, int16 h )
	: x( (float)x ), y( (float)y ), w( (float)w ), h( (float)h )
{
}

rect::rect( const vec2& top_left, const vec2& bottom_right )
	: x( top_left.x ), y( top_left.y ), w( bottom_right.x - top_left.x ), h( bottom_right.y - top_left.y )
{
}

vec2 rect::extents() const
{
	return { ( w - x ), ( h - y ) };
}

vec2 rect::top_left() const
{
	return { x, y };
}

vec2 rect::bottom_right() const
{
	return { x + w, y + h };
}

vec2 rect::get_midpoint() const
{
	return vec2( x + ( w / 2.f ), y + ( h / 2.f ) );
}

// returns a position within this rectangle that represents
// the best spot to start drawing from, given the alignment
// requested.

vec2 rect::get_pos_from_alignment( e_align align ) const
{
	vec2 pos = { x, y };

	auto mid = get_midpoint();
	auto br = bottom_right();

	if( align & align::right )
	{
		pos.x = br.x;
	}
	if( align & align::hcenter )
	{
		pos.x = mid.x;
	}

	if( align & align::bottom )
	{
		pos.y = br.y;
	}
	if( align & align::vcenter )
	{
		pos.y = mid.y;
	}

	return pos;
}

// inflates/deflates a rectangle by "val". this affects all 4 sides.

rect rect::grow( float val )
{
	x -= val;
	y -= val;
	w += val * 2.f;
	h += val * 2.f;

	return *this;
}

rect rect::shrink( float val )
{
	x += val;
	y += val;
	w -= val * 2.f;
	h -= val * 2.f;

	return *this;
}

// is "pos" within the bounds of this rectangle?

bool rect::contains_point( vec2 pos )
{
	return c2AABBtoPoint( to_c2AABB(), pos.to_c2v() );
}

// NOTE : passing in a sz of < 1.f is translated as passing in a percentage of
// the existing width or height.

rect rect::cut_left( float sz )
{
	if( sz < 1.f )
	{
		sz = w * sz;
	}

	rect result = { x, y, sz, h };

	x += sz;
	w -= sz;

	return result;
}

rect rect::cut_right( float sz )
{
	if( sz < 1.f )
	{
		sz = w * sz;
	}

	rect result = { x + ( w - sz ), y, sz, h };

	w -= sz;

	return result;
}

rect rect::cut_top( float sz )
{
	if( sz < 1.f )
	{
		sz = h * sz;
	}

	rect result = { x, y, w, sz };

	y += sz;
	h -= sz;

	return result;
}

rect rect::cut_bottom( float sz )
{
	if( sz < 1.f )
	{
		sz = h * sz;
	}

	rect result = { x, y + ( h - sz ), w, sz };

	h -= sz;

	return result;
}

// returns whatever is remaining and returns it whole

rect rect::cut()
{
	rect result = *this;

	*this = rect::zero;

	return result;
}

bool rect::operator==( const rect& rhs ) const
{
	return( fequals( rhs.x, x ) and fequals( rhs.y, y ) and fequals( rhs.w, w ) and fequals( rhs.h, h ) );
}

rect rect::operator+( const vec2& v ) const
{
	return rect( this->x + v.x, this->y + v.y, this->w, this->h );
}

rect rect::operator+=( const vec2& v )
{
	*this = *this + v;
	return *this;
}

rect rect::operator-( const vec2& v ) const
{
	return rect( this->x - v.x, this->y - v.y, this->w, this->h );
}

rect rect::operator-=( const vec2& v )
{
	*this = *this - v;
	return *this;
}

rect rect::operator+( const rect& rhs ) const
{
	return rect( x + rhs.x, y + rhs.y, w + rhs.w, h + rhs.h );
}

rect rect::operator-( const rect& rhs ) const
{
	return rect( x - rhs.x, y - rhs.y, w - rhs.w, h - rhs.h );
}

rect rect::operator*( float v ) const
{
	return rect( this->x * v, this->y * v, this->w * v, this->h * v );
}

rect rect::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

c2AABB rect::to_c2AABB() const
{
	c2AABB aabb = {};

	aabb.min.x = to_simple( x );
	aabb.min.y = to_simple( y );
	aabb.max.x = to_simple( x + w );
	aabb.max.y = to_simple( y + h  );

	return aabb;
}

rect rect::create_centered( float sz )
{
	return rect::create_centered( sz, sz );
}

rect rect::create_centered( float w, float h )
{
	auto hw = w / 2.f;
	auto hh = h / 2.f;

	return rect( -hw, -hh, w, h );
}

}
