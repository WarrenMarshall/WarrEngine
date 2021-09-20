
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const Vec2 Vec2::zero = Vec2( 0.f, 0.f );
const Vec2 Vec2::left = Vec2( -1.f, 0.f );
const Vec2 Vec2::right = Vec2( 1.f, 0.f );
const Vec2 Vec2::up = Vec2( 0.f, 1.f );
const Vec2 Vec2::down = Vec2( 0.f, -1.f );
const Vec2 Vec2::x_axis = Vec2( 1.f, 0.f );
const Vec2 Vec2::y_axis = Vec2( 0.f, 1.f );
const float_t Vec2::defaulted = -1.f;
const float_t Vec2::ignored = -2.f;

Vec2::Vec2()
	: x( 0.f ), y( 0.f )
{
}

Vec2::Vec2( int32_t x, int32_t y )
	: x( (float_t)( x ) ), y( (float_t)( y ) )
{
}

Vec2::Vec2( float_t x, float_t y )
	: x( x ), y( y )
{
}

Vec2::Vec2( std::string_view str )
{
	Tokenizer tok( str, "," );

	x = Text_Parser::float_from_str( *tok.get_next_token() );
	y = Text_Parser::float_from_str( *tok.get_next_token() );
}

Vec2::Vec2( const b2Vec2& b2v2 )
	: x( b2v2.x ), y( b2v2.y )
{
}

Vec2::Vec2( float_t v )
	: x( v ), y( v )
{
}

b2Vec2 Vec2::to_b2Vec2() const
{
	return b2Vec2( x, y );
}

c2v Vec2::to_c2v() const
{
	c2v v = { war::to_simple( x ), war::to_simple( y ) };
	return v;
}

Vec2 Vec2::to_box2d() const
{
	return Vec2( war::to_box2d( x ), war::to_box2d( y ) );
}

Vec2 Vec2::from_box2d() const
{
	return Vec2( war::from_box2d( x ), war::from_box2d( y ) );
}

Vec2 Vec2::to_simple() const
{
	return Vec2( war::to_simple( x ), war::to_simple( y ) );
}

Vec2 Vec2::from_simple() const
{
	return Vec2( war::from_simple( x ), war::from_simple( y ) );
}

bool Vec2::operator==( const Vec2& v ) const
{
	return ( fequals( this->x, v.x ) and fequals( this->y, v.y ) );
}

bool Vec2::operator!=( const Vec2& v ) const
{
	return !( fequals( this->x, v.x ) and fequals( this->y, v.y ) );
}

Vec2 Vec2::operator+( const Vec2& v ) const
{
	return Vec2( this->x + v.x, this->y + v.y );
}

Vec2 Vec2::operator+=( const Vec2& v )
{
	*this = *this + v;
	return *this;
}

Vec2 Vec2::operator-( const Vec2& v ) const
{
	return Vec2( this->x - v.x, this->y - v.y );
}

Vec2 Vec2::operator-=( const Vec2& v )
{
	*this = *this - v;
	return *this;
}

Vec2 Vec2::operator-() const
{
	return *this * -1.f;
}

Vec2 Vec2::operator*( float_t v ) const
{
	return Vec2( this->x * v, this->y * v );
}

Vec2 Vec2::operator*( Vec2 v ) const
{
	return Vec2( x * v.x, y * v.y );
}

Vec2 Vec2::operator*=( float_t v )
{
	*this = *this * v;
	return *this;
}

Vec2 Vec2::operator*=( Vec2 v )
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

Vec2 Vec2::operator/( float_t v ) const
{
	return Vec2( this->x / v, this->y / v );
}

Vec2 Vec2::operator/=( float_t v )
{
	*this = *this / v;
	return *this;
}

float_t Vec2::get_size() const
{
	return glm::length( glm::vec2( x, y ) );
}

float_t Vec2::get_size_fast() const
{
	return ( x * x ) + ( y * y );
}

bool Vec2::is_zero() const
{
	return fequals( x, 0.f ) and fequals( y, 0.f );
}

bool Vec2::is_nan() const
{
	return isnan( x ) or isnan( y );
}

void Vec2::operator=( const float& v )
{
	x = y = v;
}

void Vec2::operator=( const Vec2& v )
{
	x = v.x;
	y = v.y;
}

Vec2 Vec2::normalize( const Vec2& v )
{
	auto nv = glm::normalize( glm::vec2( v.x, v.y ) );
	return Vec2( nv.x, nv.y );
}

Vec2 Vec2::normalize()
{
	*this = Vec2::normalize( *this );
	return *this;
}

Vec2 Vec2::normalize() const
{
	return Vec2::normalize( *this );
}

Vec2 Vec2::clamp( float_t value )
{
	Range r( value );
	x = r.clamp_value( x );
	y = r.clamp_value( y );
	return *this;
}


// takes an angle, in degrees, and returns a unit vector for it

Vec2 Vec2::dir_from_angle( float_t angle )
{
	Vec2 v;

	float_t rad = glm::radians( angle );

	v.x = glm::sin( rad );
	v.y = -glm::cos( rad );

	return v;
}

float_t Vec2::angle_from_dir( const Vec2& dir )
{
	return glm::atan( dir.x, -dir.y ) * 180.f / glm::pi<float_t>();
}

// call this function instead of angle_from_dir if you'd like the resulting
// angle to be positive and clamped to be within 0-360.

float_t Vec2::clamped_angle_from_dir( const Vec2& dir )
{
	return glm::mod<float_t>( angle_from_dir( dir ), 360.f );
}

// computes the reflection angle of "v" across the normal "n"

Vec2 Vec2::reflect_across_normal( const Vec2& v, const Vec2& n )
{
	auto nn = n.normalize();

	auto reflected_dir = glm::reflect( glm::vec2( v.x, v.y ), glm::vec2( nn.x, nn.y ) );
	return Vec2( reflected_dir.x, reflected_dir.y );
}

Vec2 Vec2::snap_to_int( const Vec2& v )
{
	return Vec2( war::snap_to_int( v.x ), war::snap_to_int( v.y ) );
}

Vec2 Vec2::snap_to_int()
{
	return Vec2::snap_to_int( *this );
}

Vec2 Vec2::cross( const Vec2& a, const Vec2& b )
{
	auto v = glm::cross( glm::vec3( a.x, a.y, 0.f ), glm::vec3( b.x, b.y, 0.f ) );
	return Vec2( v.x, v.y );
}

float_t Vec2::dot( const Vec2& a, const Vec2& b )
{
	return glm::dot(
		glm::normalize( glm::vec2( a.x, a.y ) ),
		glm::normalize( glm::vec2( b.x, b.y ) )
	);
}

// returns a w_uv representing the uv tiling factors needed to tile "texture"
// inside of "rc" a natural number of times in both the U and V directions.

Vec2 Vec2::compute_uv_tiling( const Texture_Asset* texture, const Rect& rc )
{
	return Vec2( rc.w / texture->rc.w, rc.h / texture->rc.h );
}

}
