
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

w_range::w_range( std::string str )
{
	w_tokenizer tok( str, ',' );

	min = w_parser::float_from_str( tok.get_next_token() );
	max = w_parser::float_from_str( tok.get_next_token() );
}

float w_range::get_value()
{
	return min + ( ( max - min ) * w_random::getf() );
}

// ----------------------------------------------------------------------------

w_rect::w_rect()
{
}

w_rect::w_rect( float x, float y, float w, float h )
	: x( x ), y( y ), w( w ), h( h )
{
}

// ----------------------------------------------------------------------------

w_color::w_color( const w_color& other )
	: r( other.r ), g( other.g ), b( other.b ), a( other.a )
{
}

w_color::w_color( float r, float g, float b, float a )
	: r( r ), g( g ), b( b ), a( a )
{
}

w_color::w_color( std::string str )
{
	w_tokenizer tok( str, ',', "1.0f" );

	r = w_parser::float_from_str( tok.get_next_token() );
	g = w_parser::float_from_str( tok.get_next_token() );
	b = w_parser::float_from_str( tok.get_next_token() );
	a = w_parser::float_from_str( tok.get_next_token() );
}

w_color w_color::from_string( std::string text )
{
	w_tokenizer tok( text, ',' );

	r = w_parser::float_from_str( tok.get_next_token() );
	g = w_parser::float_from_str( tok.get_next_token() );
	b = w_parser::float_from_str( tok.get_next_token() );
	a = w_parser::float_from_str( tok.get_next_token() );

	return *this;
}

// ----------------------------------------------------------------------------

const w_vec2 w_vec2::zero = w_vec2( 0, 0 );

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

w_vec2::w_vec2( std::string str )
{
	w_tokenizer tok( str, ',' );

	x = w_parser::float_from_str( tok.get_next_token() );
	y = w_parser::float_from_str( tok.get_next_token() );
}

w_vec2 w_vec2::operator+( const w_vec2& v )
{
	return w_vec2( x + v.x, y + v.y );
}

w_vec2 w_vec2::operator-( const w_vec2& v )
{
	return w_vec2( x - v.x, y - v.y );
}

w_vec2 w_vec2::operator=( const w_vec2& v )
{
	x = v.x;
	y = v.y;

	return *this;
}

w_vec2 w_vec2::scale( w_vec2 a, float scale )
{
	return w_vec2( a.x * scale, a.y * scale );
}

float w_vec2::get_size_squared( w_vec2 a )
{
	return sqrt( w_vec2::get_size( a ) );
}

float w_vec2::get_size( w_vec2 a )
{
	return (a.x * a.x) + (a.y * a.y);
}

float w_vec2::get_distance_between( w_vec2 a, w_vec2 b )
{
	return w_vec2::get_size_squared( a - b );
}

w_vec2 w_vec2::normalize( w_vec2 a )
{
	float sz = w_vec2::get_size_squared( a );
	return w_vec2( a.x / sz, a.y / sz );
}

// takes an angle, in degrees, and returns a unit vector for it

w_vec2 w_vec2::from_angle( float angle )
{
	w_vec2 v;

	float rad = deg2rad( angle );
	v.x = -std::sinf( rad );
	v.y = std::cosf( rad );

	return v;
}

// ----------------------------------------------------------------------------

const w_vec3 w_vec3::zero = w_vec3( 0, 0, 0 );

w_vec3::w_vec3()
	: x( 0.0f ), y( 0.0f ), z( 0.0f )
{
}

w_vec3::w_vec3( const w_vec3& v )
	: x( v.x ), y( v.y ), z( v.z )
{
}

w_vec3::w_vec3( float x, float y, float z )
	: x( x ), y( y ), z( z )
{
}

w_vec3::w_vec3( int x, int y, int z )
	: x( (float)x ), y( (float) y ), z( (float) z )
{
}

w_vec3::w_vec3( std::string str )
{
	w_tokenizer tok( str, ',' );

	x = w_parser::float_from_str( tok.get_next_token() );
	y = w_parser::float_from_str( tok.get_next_token() );
	z = w_parser::float_from_str( tok.get_next_token() );
}

w_vec3 w_vec3::operator+( const w_vec3& v )
{
	return w_vec3( x + v.x, y + v.y, z + v.z );
}

w_vec3 w_vec3::operator-( const w_vec3& v )
{
	return w_vec3( x - v.x, y - v.y, z - v.z );
}

w_vec3 w_vec3::operator=( const w_vec3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

w_vec3 w_vec3::operator*( const float& f )
{
	return w_vec3( x * f, y * f, z * f );
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

w_vec3 w_vec3::normalize( w_vec3 a )
{
	float sz = w_vec3::get_size_squared( a );
	return w_vec3( a.x / sz, a.y / sz, a.z / sz );
}

w_vec3 w_vec3::from_angle( float angle )
{
	w_vec3 v;

	float rad = deg2rad( angle );
	v.x = -std::sinf( rad );
	v.y = std::cosf( rad );
	v.z = 0.0f;

	return v;
}

// ----------------------------------------------------------------------------

w_bbox::w_bbox()
{
	reset();
}

void w_bbox::add( w_vec3 vtx )
{
	min.x = w_min( min.x, vtx.x );
	min.y = w_min( min.y, vtx.y );
	min.z = w_min( min.z, vtx.z );

	max.x = w_max( max.x, vtx.x );
	max.y = w_max( max.y, vtx.y );
	max.z = w_max( max.z, vtx.z );
}

void w_bbox::reset()
{
	min.x = min.y = min.z = std::numeric_limits<float>::max();
	max.x = max.y = max.z = std::numeric_limits<float>::min();
}
