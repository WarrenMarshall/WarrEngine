
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const Rect Rect::zero = Rect( 0, 0, 0, 0 );

Rect::Rect( f32 x, f32 y )
	: x( x ), y( y )
{
}

Rect::Rect( f32 x, f32 y, f32 w, f32 h )
	: x( x ), y( y ), w( w ), h( h )
{
}

Rect::Rect( i32 x, i32 y, i32 w, i32 h )
	: x( (f32)x ), y( (f32)y ), w( (f32)w ), h( (f32)h )
{
}

Rect::Rect( const Vec2& top_left, const Vec2& width_height )
	: x( top_left.x ), y( top_left.y ), w( width_height.x ), h( width_height.y )
{
}

Rect::Rect( const Vec2& top_left, f32 width, f32 height )
	: x( top_left.x ), y( top_left.y ), w( width ), h( height )
{
}

Rect::Rect( const Vec2& pos )
	: x( pos.x ), y( pos.y ), w( 0.f ), h( 0.f )
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

Vec2 Rect::get_pos_from_alignment( e_align align ) const
{
	Vec2 pos = { x, y };

	auto mid = get_midpoint();
	auto br = bottom_right();

	if( (i32)(align & e_align::right) )
	{
		pos.x = br.x;
	}
	if( (i32)( align & e_align::hcenter) )
	{
		pos.x = mid.x;
	}

	if( (i32)( align & e_align::bottom) )
	{
		pos.y = br.y;
	}
	if( (i32)( align & e_align::vcenter) )
	{
		pos.y = mid.y;
	}

	return pos;
}

// inflates/deflates a rectangle by "val". this affects all 4 sides.

Rect Rect::grow( f32 val )
{
	x -= val;
	y -= val;
	w += val * 2.f;
	h += val * 2.f;

	return *this;
}

Rect Rect::shrink( f32 val )
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

Rect Rect::cut_left( f32 sz )
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

Rect Rect::cut_right( f32 sz )
{
	if( sz < 1.f )
	{
		sz = w * sz;
	}

	Rect result = { x + ( w - sz ), y, sz, h };

	w -= sz;

	return result;
}

Rect Rect::cut_top( f32 sz )
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

Rect Rect::cut_bottom( f32 sz )
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

// creates 4 smaller rectangles from this one and returns them

std::vector<Rect> Rect::subdivide() const
{
	std::vector<Rect> subrects;
	subrects.reserve( 4 );

	auto hw = w / 2.f;
	auto hh = h / 2.f;

	subrects.emplace_back( Rect( x, y, hw, hh ) );
	subrects.emplace_back( Rect( x + hw, y, hw, hh ) );
	subrects.emplace_back( Rect( x + hw, y + hh, hw, hh ) );
	subrects.emplace_back( Rect( x, y + hh, hw, hh ) );

	return subrects;
}

// generates a random location within this rectangle

Vec2 Rect::find_random_pos_within()
{
	return Vec2( x + ( w * Random::getf() ), y + ( h * Random::getf() ) );
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

Rect Rect::operator*( f32 v ) const
{
	return Rect( this->x * v, this->y * v, this->w * v, this->h * v );
}

Rect Rect::operator*=( f32 v )
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
	aabb.max.y = to_simple( y + h );

	return aabb;
}

c2AABB Rect::as_c2AABB() const
{
	c2AABB aabb = {};

	aabb.min.x = x;
	aabb.min.y = y;
	aabb.max.x = x + w;
	aabb.max.y = y + h;

	return aabb;
}

Rect Rect::create_centered( f32 sz )
{
	return Rect::create_centered( sz, sz );
}

Rect Rect::create_centered( f32 w, f32 h )
{
	auto hw = w / 2.f;
	auto hh = h / 2.f;

	return Rect( -hw, -hh, w, h );
}

i32 Rect::area()
{
	return (i32)( w * h );
}

}
