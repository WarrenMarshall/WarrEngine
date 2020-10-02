
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_keyframe::w_keyframe( const w_keyframe& other )
	: pct_marker( other.pct_marker ), float_value( other.float_value ), color_value( other.color_value )
{
}

w_keyframe::w_keyframe( float pct_marker, float value )
	: pct_marker( pct_marker ), float_value( value )
{
}

w_keyframe::w_keyframe( float pct_marker, w_color value )
	: pct_marker( pct_marker ), color_value( value )
{
}

// ----------------------------------------------------------------------------

w_timeline::w_timeline( e_timeline_type type )
	: type( type )
{
	keyframes.clear();
}

w_timeline::w_timeline( const w_timeline& other )
	: type( other.type ), keyframes( other.keyframes )
{
}

w_timeline* w_timeline::kf_clear()
{
	keyframes.clear();
	return this;
}

w_timeline* w_timeline::kf_add( w_keyframe keyframe )
{
	keyframes.emplace_back( std::move( keyframe ) );
	return this;
}

/*
	computes a value on the timeline between 0-1, based
	on the "pct_on_timeline" pass in.
*/
void w_timeline::get_value( float pct_on_timeline, float* value )
{
	assert( pct_on_timeline >= 0.0f && pct_on_timeline <= 1.0f );

	// figure out which keyframe is the one we are approaching.
	// this will be the one we are less than or equal to.

	int kf_max = 0;
	for( ; kf_max < keyframes.size()-1; ++kf_max )
	{
		if( keyframes[kf_max].pct_marker > pct_on_timeline )
		{
			break;
		}
	}

	int kf_min = kf_max - 1;

	// the range of percentages within the min/max keyframes
	float pct_range = keyframes[kf_max].pct_marker - keyframes[kf_min].pct_marker;

	// the pct we are at within the min/max keyframes
	float pct_within = (pct_on_timeline - keyframes[kf_min].pct_marker) / pct_range;

	// the value represented by that pct_within
	*value = keyframes[kf_min].float_value + ( (keyframes[kf_max].float_value - keyframes[kf_min].float_value ) * pct_within );
}

void w_timeline::get_value( float pct_on_timeline, w_color* value )
{
	assert( pct_on_timeline >= 0.0f && pct_on_timeline <= 1.0f );

	// figure out which keyframe is the one we are approaching.
	// this will be the one we are less than or equal to.

	int kf_max = 0;
	for( ; kf_max < keyframes.size()-1; ++kf_max )
	{
		if( keyframes[kf_max].pct_marker > pct_on_timeline )
		{
			break;
		}
	}

	int kf_min = kf_max - 1;

	// the range of percentages within the min/max keyframes
	float pct_range = keyframes[kf_max].pct_marker - keyframes[kf_min].pct_marker;

	// the pct we are at within the min/max keyframes
	float pct_within = (pct_on_timeline - keyframes[kf_min].pct_marker) / pct_range;

	// the value represented by that pct_within
	value->r = keyframes[kf_min].color_value.r + ( ( keyframes[kf_max].color_value.r - keyframes[kf_min].color_value.r ) * pct_within );
	value->g = keyframes[kf_min].color_value.g + ( ( keyframes[kf_max].color_value.g - keyframes[kf_min].color_value.g ) * pct_within );
	value->b = keyframes[kf_min].color_value.b + ( ( keyframes[kf_max].color_value.b - keyframes[kf_min].color_value.b ) * pct_within );
	value->a = keyframes[kf_min].color_value.a + ( ( keyframes[kf_max].color_value.a - keyframes[kf_min].color_value.a ) * pct_within );
}

// ----------------------------------------------------------------------------

#undef min
#undef max

w_range::w_range( float min, float max )
	: min( min ), max( max )
{
}

w_range::w_range( std::string_view str )
{
	w_tokenizer tok( str, ',' );

	min = w_parser::float_from_str( *tok.get_next_token() );
	max = w_parser::float_from_str( *tok.get_next_token() );
}

float w_range::get_value()
{
	return min + ( ( max - min ) * w_random::getf() );
}

// ----------------------------------------------------------------------------

const w_rect w_rect::zero = w_rect( 0, 0, 0, 0 );

w_rect::w_rect( float x, float y )
	: x( x ), y( y )
{
}

w_rect::w_rect( float x, float y, float w, float h )
	: x( x ), y( y ), w( w ), h( h )
{
}

w_rect::w_rect( w_vec2 top_left, w_vec2 bottom_right )
	: x( top_left.x ), y( top_left.y ), w( bottom_right.x - top_left.x ), h( bottom_right.y - top_left.y )
{
}

// ----------------------------------------------------------------------------

const w_color w_color::white = w_color( 1.0f, 1.0f, 1.0f );
const w_color w_color::black = w_color( 0.0f, 0.0f, 0.0f );
const w_color w_color::red = w_color( 1.0f, 0.0f, 0.0f );
const w_color w_color::green = w_color( 0.0f, 1.0f, 0.0f );
const w_color w_color::blue = w_color( 0.0f, 0.0f, 1.0f );
const w_color w_color::light_blue = w_color( 0.5f, 0.5f, 1.0f );
const w_color w_color::orange = w_color( 1.0f, 0.5f, 0.0f );
const w_color w_color::yellow = w_color( 1.0f, 1.0f, 0.0f );
const w_color w_color::teal = w_color( 0.3f, 0.8f, 1.0f );
const w_color w_color::dark_teal = w_color( 0.2f, 0.4f, 0.5f );
const w_color w_color::dark_grey = w_color( 0.25f, 0.25f, 0.25f );
const w_color w_color::grey = w_color( 0.5f, 0.5f, 0.5f );
const w_color w_color::light_grey = w_color( 0.75f, 0.75f, 0.75f );
const w_color w_color::light_green = w_color( 0.5f, 1.0f, 0.5f );
const w_color w_color::dark_green = w_color( 0.25f, 0.5f, 0.25f );
const w_color w_color::magenta = w_color( 0.96f, 0.32f, 0.65f );

w_color::w_color( const w_color& other ) = default;
w_color::w_color( float r, float g, float b, float a )
	: r( r ), g( g ), b( b ), a( a )
{
}

w_color::w_color( int r, int g, int b, int a )
	: w_color( r * byte_color_to_float, g * byte_color_to_float, b * byte_color_to_float, a * byte_color_to_float )
{
}

w_color::w_color( std::string_view str )
{
	w_tokenizer tok( str, ',' );

	r = w_parser::float_from_str( *tok.get_next_token() );
	g = w_parser::float_from_str( *tok.get_next_token() );
	b = w_parser::float_from_str( *tok.get_next_token() );
	auto alpha = tok.get_next_token();
	a = alpha.has_value() ? w_parser::float_from_str( *alpha ) : 1.0f;
}

void w_color::scale( w_color& color, float s )
{
	color.r *= s;
	color.g *= s;
	color.b *= s;
}

// ----------------------------------------------------------------------------

const w_vec2 w_vec2::zero = w_vec2( 0, 0 );

// generates a random point on a unit sphere.
w_vec2 w_vec2::get_random_unit()
{
	return w_vec2( -1.0f + (w_random::getf() * 2.0f), -1.0f + (w_random::getf() * 2.0f) ).normalize();
}

w_vec2::w_vec2()
	: x( 0.0f ), y( 0.0f )
{
}

w_vec2::w_vec2( int x, int y )
	: x( static_cast<float>( x ) ), y( static_cast<float>( y ) )
{
}

w_vec2::w_vec2( float x, float y )
	: x( x ), y( y )
{
}

w_vec2::w_vec2( std::string_view str )
{
	w_tokenizer tok( str, ',' );

	x = w_parser::float_from_str( *tok.get_next_token() );
	y = w_parser::float_from_str( *tok.get_next_token() );
}

w_vec2::w_vec2( b2Vec2 b2v2 )
	: x( b2v2.x ), y( b2v2.y )
{
}

w_vec2 w_vec2::normalize()
{
	*this = w_vec2::normalize( *this );
	return *this;
}

w_vec2 w_vec2::to_b2d()
{
	return w_vec2( ::to_b2d( x ), ::to_b2d( y ) );
}

w_vec2 w_vec2::from_b2d()
{
	return w_vec2( ::from_b2d( x ), ::from_b2d( y ) );
}

bool w_vec2::operator==( w_vec2 v )
{
	return ( fequals( this->x, v.x ) && fequals( this->y, v.y ) );
}

bool w_vec2::operator!=( w_vec2 v )
{
	return !( fequals( this->x, v.x ) && fequals( this->y, v.y ) );
}

w_vec2 w_vec2::operator+( w_vec2 v )
{
	return w_vec2( this->x + v.x, this->y + v.y );
}

w_vec2 w_vec2::operator+=( w_vec2 v )
{
	*this = *this + v;
	return *this;
}

w_vec2 w_vec2::operator-( w_vec2 v )
{
	return w_vec2( this->x - v.x, this->y - v.y );
}

w_vec2 w_vec2::operator-=( w_vec2 v )
{
	*this = *this - v;
	return *this;
}

w_vec2 w_vec2::operator*( float v )
{
	return w_vec2( this->x * v, this->y * v );
}

w_vec2 w_vec2::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

float w_vec2::get_size_squared()
{
	return sqrt( w_vec2::get_size() );
}

float w_vec2::get_size()
{
	return ( x * x ) + ( y * y );
}

float w_vec2::get_distance_between( w_vec2 a, w_vec2 b )
{
	return ( a - b ).get_size_squared();
}

w_vec2 w_vec2::normalize( w_vec2 a )
{
	float sz = a.get_size_squared();
	return w_vec2( a.x / sz, a.y / sz );
}

// takes an angle, in degrees, and returns a unit vector for it

w_vec2 w_vec2::from_angle( float angle )
{
	w_vec2 v;

	float rad = glm::radians( angle );
	v.x = std::sinf( rad );
	v.y = -std::cosf( rad );

	return v;
}

float w_vec2::to_angle( w_vec2 a )
{
	float angle = atan2( a.y, a.x ) * 180.0f / W_PI;

	// massage the resulting angle into a range that this engine likes.
	angle += 90.0f;
	if( angle < 0.0f ) angle += 360.0f;

	return angle;
}

// computes the reflection angle of "v" across the normal "n"

w_vec2 w_vec2::reflect( w_vec2 v, w_vec2 n )
{
	glm::vec3 rdir = glm::reflect( static_cast<glm::vec3>( v ), static_cast<glm::vec3>( n ) );
	return w_vec2( rdir.x, rdir.y );
}

// ----------------------------------------------------------------------------

const w_vec3 w_vec3::zero = w_vec3( 0, 0, 0 );

w_vec3::w_vec3()
	: x( 0.0f ), y( 0.0f ), z( 0.0f )
{
}

w_vec3::w_vec3( const w_vec3& v ) = default;

w_vec3::w_vec3( float x, float y, float z )
	: x( x ), y( y ), z( z )
{
}

w_vec3::w_vec3( int x, int y, int z )
	: x( (float)x ), y( (float) y ), z( (float) z )
{
}

w_vec3::w_vec3( std::string_view str )
{
	w_tokenizer tok( str, ',' );

	x = w_parser::float_from_str( *tok.get_next_token() );
	y = w_parser::float_from_str( *tok.get_next_token() );
	z = w_parser::float_from_str( *tok.get_next_token() );
}

float w_vec3::get_size_squared( w_vec3 a )
{
	return sqrt( w_vec3::get_size( a ) );
}

float w_vec3::get_size( w_vec3 a )
{
	return ( a.x * a.x ) + ( a.y * a.y ) + ( a.z * a.z );
}

float w_vec3::get_distance_between( w_vec3 a, w_vec3 b )
{
	return w_vec3::get_size_squared( a - b );
}

w_vec3 w_vec3::operator+( w_vec3 v )
{
	return w_vec3( this->x + v.x, this->y + v.y, this->z + v.z );
}

w_vec3 w_vec3::operator-( w_vec3 v )
{
	return w_vec3( this->x - v.x, this->y - v.y, this->z - v.z );
}

w_vec3 w_vec3::operator*( float v )
{
	return w_vec3( this->x * v, this->y * v, this->z * v );
}

w_vec3 w_vec3::normalize( w_vec3 a )
{
	float sz = w_vec3::get_size_squared( a );
	return w_vec3( a.x / sz, a.y / sz, a.z / sz );
}

// ----------------------------------------------------------------------------

w_bbox::w_bbox()
{
	reset();
}

void w_bbox::add( w_vec2 vtx )
{
	min.x = w_min( min.x, vtx.x );
	min.y = w_min( min.y, vtx.y );

	max.x = w_max( max.x, vtx.x );
	max.y = w_max( max.y, vtx.y );
}

void w_bbox::reset()
{
	min.x = min.y = std::numeric_limits<float>::max();
	max.x = max.y = std::numeric_limits<float>::min();
}
