
#include "master_pch.h"
#include "master_header.h"

namespace war
{

// ----------------------------------------------------------------------------

timeline_key_frame::timeline_key_frame( float pct_marker, float value )
	: pct_marker( pct_marker ), value( value )
{
}

timeline_key_frame::timeline_key_frame( float pct_marker, const color& value )
	: pct_marker( pct_marker ), value( value )
{
}

// ----------------------------------------------------------------------------

timeline::timeline( e_timeline_type type )
	: type( type )
{
	key_frames.clear();
}

timeline* timeline::clear_key_frames()
{
	key_frames.clear();
	return this;
}

timeline* timeline::add_key_frame( const timeline_key_frame& kf )
{
	key_frames.emplace_back( kf );
	return this;
}

// figure out which key_frame is the one we are approaching next
// based on a percentage indicator of where we are on the timeline.

size_t timeline::find_next_key_frame_idx_from_pct( float pct )
{
	size_t kf_next = 0;

	for( ; kf_next < key_frames.size() - 1; ++kf_next )
	{
		if( key_frames[ kf_next ].pct_marker > pct )
		{
			break;
		}
	}

	return kf_next;
}

// ----------------------------------------------------------------------------

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

// NOTE : passing in a sz of < 1.0f is translated as passing in a percentage of
// the existing width or height.

rect rect::cut_left( float sz )
{
	if( sz < 1.0f )
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
	if( sz < 1.0f )
	{
		sz = w * sz;
	}

	rect result = { x + ( w - sz ), y, sz, h };

	w -= sz;

	return result;
}

rect rect::cut_top( float sz )
{
	if( sz < 1.0f )
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
	if( sz < 1.0f )
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
	c2AABB bb = {};
	bb.min = { x, y };
	bb.max = { x + w, y + h };

	return bb;
}

// ----------------------------------------------------------------------------

const color color::white = color( 1.f, 1.f, 1.f );
const color color::black = color( 0.f, 0.f, 0.f );
const color color::dark_red = color( 0.5f, 0.f, 0.f );
const color color::red = color( 1.f, 0.f, 0.f );
const color color::dark_green = color( 0.f, 0.5f, 0.f );
const color color::green = color( 0.f, 1.f, 0.f );
const color color::blue = color( 0.f, 0.f, 1.f );
const color color::light_blue = color( 0.5f, 0.5f, 1.f );
const color color::orange = color( 1.f, 0.5f, 0.f );
const color color::yellow = color( 1.f, 1.f, 0.f );
const color color::teal = color( 0.3f, 0.8f, 1.f );
const color color::dark_teal = color( 0.2f, 0.4f, 0.5f );
const color color::dark_grey = color( 0.25f, 0.25f, 0.25f );
const color color::grey = color( 0.5f, 0.5f, 0.5f );
const color color::light_grey = color( 0.75f, 0.75f, 0.75f );
const color color::light_green = color( 0.5f, 1.f, 0.5f );
const color color::magenta = color( 0.96f, 0.32f, 0.65f );

color::color( float r, float g, float b, float a )
	: r( r ), g( g ), b( b ), a( a )
{
}

// integer values are assumed to be in the 0-255 range and are converted to float
color::color( int r, int g, int b, int a )
	: color( r* byte_color_to_float, g* byte_color_to_float, b* byte_color_to_float, a* byte_color_to_float )
{
}

// strings can contain any supported kind of data format (hex, int, or floats)
color::color( std::string& str )
{
	assert( !str.empty() );

	if( str[ 0 ] == '@' )
	{
		// strings starting with a '@' char are palette indices
		tokenizer tok( str, "@" );
		int idx = string_util::to_int( std::string( tok.get_next_token().value_or( "0" ) ) );
		*this = make_color( idx );
	}
	else if( str[ 0 ] == '$' )
	{
		// strings starting with a '$' char are hex values
		assert( str.length() == 7 );

		r = string_util::to_uint( "$" + str.substr( 1, 2 ) ) * byte_color_to_float;
		g = string_util::to_uint( "$" + str.substr( 3, 2 ) ) * byte_color_to_float;
		b = string_util::to_uint( "$" + str.substr( 5, 2 ) ) * byte_color_to_float;
	}
	else
	{
		// in case this is a set of values, remove the surrounding braces.
		str.erase( std::remove( str.begin(), str.end(), '[' ), str.end() );
		str.erase( std::remove( str.begin(), str.end(), ']' ), str.end() );

		// if string contains a "%" marker anywhere, we assume this to
		// be a set of integer values. Otherwise, they are treated as floats.
		//
		// note : you can't mix-and-match

		if( str.find_first_of( '%' ) != std::string::npos )
		{
			str.erase( std::remove( str.begin(), str.end(), '%' ), str.end() );
			tokenizer tok( str, "," );

			r = text_parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			g = text_parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			b = text_parser::int_from_str( *tok.get_next_token() ) * byte_color_to_float;
			a = text_parser::int_from_str( tok.get_next_token().value_or( "1.f" ) ) * byte_color_to_float;
		}
		else
		{
			// remove any 'f' postfixes that may have been added to the
			// color values out of programmer habit

			str.erase( std::remove( str.begin(), str.end(), 'f' ), str.end() );
			tokenizer tok( str, "," );

			r = text_parser::float_from_str( *tok.get_next_token() );
			g = text_parser::float_from_str( *tok.get_next_token() );
			b = text_parser::float_from_str( *tok.get_next_token() );
			a = text_parser::float_from_str( tok.get_next_token().value_or( "1.f" ) );
		}
	}
}

color color::operator*( float v ) const
{
	return color( this->r * v, this->g * v, this->b * v );
}

color color::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

color color::operator-( color v ) const
{
	return color( this->r - v.r, this->g - v.g, this->b - v.b, this->a - v.a );
}

color color::operator-=( color v )
{
	*this = *this - v;
	return *this;
}

color color::operator+( color v ) const
{
	return color( this->r + v.r, this->g + v.g, this->b + v.b, this->a + v.a );
}

color color::operator+=( color v )
{
	*this = *this + v;
	return *this;
}

void color::scale( color& color, float s )
{
	color.r *= s;
	color.g *= s;
	color.b *= s;
}

color make_color( const color& clr, float alpha )
{
	return color( clr.r, clr.g, clr.b, alpha );
}

color make_color( e_pal pal_idx, float alpha )
{
	return make_color( render::palette[ pal_idx ], alpha );
}

// ----------------------------------------------------------------------------

const vec2 vec2::zero = vec2( 0.f, 0.f );
const vec2 vec2::left = vec2( -1.f, 0.f );
const vec2 vec2::right = vec2( 1.f, 0.f );
const vec2 vec2::up = vec2( 0.f, -1.f );
const vec2 vec2::down = vec2( 0.f, 1.f );
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
	c2v v = { x, y };
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

vec2 vec2::operator*( float v ) const
{
	return vec2( this->x * v, this->y * v );
}

vec2 vec2::operator*=( float v )
{
	*this = *this * v;
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

float vec2::get_size_squared() const
{
	return glm::sqrt( vec2::get_size_fast() );
}

float vec2::get_size_fast() const
{
	return ( x * x ) + ( y * y );
}

bool vec2::is_zero() const
{
	return fequals( x + y, 0.f );
}

void vec2::operator=( const float& v )
{
	x = y = v;
}

float vec2::get_distance_between( const vec2& a, const vec2& b )
{
	return ( a - b ).get_size_squared();
}

// normalizes the vec2 in place, and returns itself in case the caller
// wants to handle it as a return value instead

vec2 vec2::normalize()
{
	return *this / get_size_squared();
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
	auto angle = glm::atan( dir.x, -dir.y ) * 180.f / glm::pi<float>();

	// 5/2/2021 - uncomment this if values outside of 0-360 prove to be unusable
	// for some reason. i like the purity of not having to do this.

	// angle = glm::mod<float>( angle, 360.f );

	return angle;
}

// computes the reflection angle of "v" across the normal "n"

vec2 vec2::reflect_across_normal( const vec2& v, const vec2& n )
{
	glm::vec3 rdir = glm::reflect( glm::vec3( v.x, v.y, 0.f ), glm::vec3( n.x, n.y, 0.f ) );
	return vec2( rdir.x, rdir.y );
}

// returns a w_uv representing the uv tiling factors needed to tile "texture"
// inside of "rc" a natural number of times in both the U and V directions.

vec2 vec2::compute_uv_tiling( const texture_asset* texture, const rect& rc )
{
	return vec2( rc.w / texture->rc.w, rc.h / texture->rc.h );
}

// ----------------------------------------------------------------------------

const vec3 vec3::zero = vec3( 0.f, 0.f, 0.f );

vec3::vec3()
	: x( 0.f ), y( 0.f ), z( 0.f )
{
}

vec3::vec3( int x, int y, int z )
	: x( (float)( x ) ), y( (float)( y ) ), z( (float)( z ) )
{
}

vec3::vec3( float x, float y, float z )
	: x( x ), y( y ), z( z )
{
}

vec3::vec3( std::string_view str )
{
	tokenizer tok( str, "," );

	x = text_parser::float_from_str( *tok.get_next_token() );
	y = text_parser::float_from_str( *tok.get_next_token() );
	z = text_parser::float_from_str( *tok.get_next_token() );
}

// ----------------------------------------------------------------------------

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
