
#include "master_pch.h"
#include "master_header.h"

namespace war
{

const vec2 vec2::zero = vec2( 0.f, 0.f );
const vec2 vec2::left = vec2( -1.f, 0.f );
const vec2 vec2::right = vec2( 1.f, 0.f );
const vec2 vec2::up = vec2( 0.f, 1.f );
const vec2 vec2::down = vec2( 0.f, -1.f );
const vec2 vec2::x_axis = vec2( 1.f, 0.f );
const vec2 vec2::y_axis = vec2( 0.f, 1.f );
const float vec2::defaulted = -1.f;
const float vec2::ignored = -2.f;

vec2::vec2()
	: x( 0.f ), y( 0.f )
{
}

vec2::vec2( int x, int y )
	: x( (float)( x ) ), y( (float)( y ) )
{
}

vec2::vec2( float x, float y )
	: x( x ), y( y )
{
}

vec2::vec2( std::string_view str )
{
	tokenizer tok( str, "," );

	x = text_parser::float_from_str( *tok.get_next_token() );
	y = text_parser::float_from_str( *tok.get_next_token() );
}

vec2::vec2( const b2Vec2& b2v2 )
	: x( b2v2.x ), y( b2v2.y )
{
}

vec2::vec2( float v )
	: x( v ), y( v )
{
}

b2Vec2 vec2::to_b2Vec2() const
{
	return b2Vec2( x, y );
}

c2v vec2::to_c2v() const
{
	c2v v = { war::to_simple( x ), war::to_simple( y ) };
	return v;
}

vec2 vec2::to_box2d() const
{
	return vec2( war::to_box2d( x ), war::to_box2d( y ) );
}

vec2 vec2::from_box2d() const
{
	return vec2( war::from_box2d( x ), war::from_box2d( y ) );
}

vec2 vec2::to_simple() const
{
	return vec2( war::to_simple( x ), war::to_simple( y ) );
}

vec2 vec2::from_simple() const
{
	return vec2( war::from_simple( x ), war::from_simple( y ) );
}

bool vec2::operator==( const vec2& v ) const
{
	return ( fequals( this->x, v.x ) and fequals( this->y, v.y ) );
}

bool vec2::operator!=( const vec2& v ) const
{
	return !( fequals( this->x, v.x ) and fequals( this->y, v.y ) );
}

vec2 vec2::operator+( const vec2& v ) const
{
	return vec2( this->x + v.x, this->y + v.y );
}

vec2 vec2::operator+=( const vec2& v )
{
	*this = *this + v;
	return *this;
}

vec2 vec2::operator-( const vec2& v ) const
{
	return vec2( this->x - v.x, this->y - v.y );
}

vec2 vec2::operator-=( const vec2& v )
{
	*this = *this - v;
	return *this;
}

vec2 vec2::operator-() const
{
	return *this * -1.f;
}

vec2 vec2::operator*( float v ) const
{
	return vec2( this->x * v, this->y * v );
}

vec2 vec2::operator*( vec2 v ) const
{
	return vec2( x * v.x, y * v.y );
}

vec2 vec2::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

vec2 vec2::operator*=( vec2 v )
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

vec2 vec2::operator/( float v ) const
{
	return vec2( this->x / v, this->y / v );
}

vec2 vec2::operator/=( float v )
{
	*this = *this / v;
	return *this;
}

float vec2::get_size() const
{
	return glm::length( glm::vec2( x, y ) );
}

float vec2::get_size_fast() const
{
	return ( x * x ) + ( y * y );
}

bool vec2::is_zero() const
{
	return fequals( x, 0.f ) and fequals( y, 0.f );
}

bool vec2::is_nan() const
{
	return isnan( x ) or isnan( y );
}

void vec2::operator=( const float& v )
{
	x = y = v;
}

void vec2::operator=( const vec2& v )
{
	x = v.x;
	y = v.y;
}

vec2 vec2::normalize( const vec2& v )
{
	auto nv = glm::normalize( glm::vec2( v.x, v.y ) );
	return vec2( nv.x, nv.y );
}

vec2 vec2::normalize()
{
	*this = vec2::normalize( *this );
	return *this;
}

vec2 vec2::normalize() const
{
	return vec2::normalize( *this );
}

vec2 vec2::clamp( float value )
{
	range r( value );
	x = r.clamp_value( x );
	y = r.clamp_value( y );
	return *this;
}


// takes an angle, in degrees, and returns a unit vector for it

vec2 vec2::dir_from_angle( float angle )
{
	vec2 v;

	float rad = glm::radians( angle );

	v.x = glm::sin( rad );
	v.y = -glm::cos( rad );

	return v;
}

float vec2::angle_from_dir( const vec2& dir )
{
	return glm::atan( dir.x, -dir.y ) * 180.f / glm::pi<float>();
}

// call this function instead of angle_from_dir if you'd like the resulting
// angle to be positive and clamped to be within 0-360.

float vec2::clamped_angle_from_dir( const vec2& dir )
{
	return glm::mod<float>( angle_from_dir( dir ), 360.f );
}

// computes the reflection angle of "v" across the normal "n"

vec2 vec2::reflect_across_normal( const vec2& v, const vec2& n )
{
	auto nn = n.normalize();

	auto reflected_dir = glm::reflect( glm::vec2( v.x, v.y ), glm::vec2( nn.x, nn.y ) );
	return vec2( reflected_dir.x, reflected_dir.y );
}

vec2 vec2::snap_to_int( const vec2& v )
{
	return vec2( war::snap_to_int( v.x ), war::snap_to_int( v.y ) );
}

vec2 vec2::snap_to_int()
{
	return vec2::snap_to_int( *this );
}

vec2 vec2::cross( const vec2& a, const vec2& b)
{
	auto v = glm::cross( glm::vec3( a.x, a.y, 0.f ), glm::vec3( b.x, b.y, 0.f ) );
	return vec2( v.x, v.y );
}

float vec2::dot( const vec2& a, const vec2& b )
{
	return glm::dot(
		glm::normalize( glm::vec2( a.x, a.y ) ),
		glm::normalize( glm::vec2( b.x, b.y ) )
	);
}

// returns a w_uv representing the uv tiling factors needed to tile "texture"
// inside of "rc" a natural number of times in both the U and V directions.

vec2 vec2::compute_uv_tiling( const texture_asset* texture, const rect& rc )
{
	return vec2( rc.w / texture->rc.w, rc.h / texture->rc.h );
}

}
