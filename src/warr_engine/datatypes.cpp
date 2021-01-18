
#include "master_pch.h"
#include "master_header.h"

// ----------------------------------------------------------------------------

w_keyframe::w_keyframe( float pct_marker, float value )
	: pct_marker( pct_marker ), float_value( value )
{
}

w_keyframe::w_keyframe( float pct_marker, const w_color& value )
	: pct_marker( pct_marker ), color_value( value )
{
}

// ----------------------------------------------------------------------------

w_timeline::w_timeline( e_timeline_type type )
	: type( type )
{
	keyframes.clear();
}

w_timeline* w_timeline::kf_clear()
{
	keyframes.clear();
	return this;
}

w_timeline* w_timeline::kf_add( const w_keyframe& keyframe )
{
	keyframes.emplace_back( std::move( keyframe ) );
	return this;
}

// figure out which keyframe is the one we are approaching next
// based on a percentage indicator of where we are on the timeline.

size_t w_timeline::find_next_keyframe_idx_from_pct( float pct )
{
	size_t kf_max = 0;

	for( ; kf_max < keyframes.size() - 1; ++kf_max )
	{
		if( keyframes[ kf_max ].pct_marker > pct )
		{
			break;
		}
	}

	return kf_max;
}

/*
	computes a value on the timeline between 0-1, based
	on the "pct_on_timeline" pass in.
*/
void w_timeline::get_value( float pct_on_timeline, float* value )
{
	assert( pct_on_timeline >= 0.0f && pct_on_timeline <= 1.0f );

	auto kf_max = find_next_keyframe_idx_from_pct( pct_on_timeline );
	size_t kf_min = kf_max - 1;

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

	auto kf_max = find_next_keyframe_idx_from_pct( pct_on_timeline );
	size_t kf_min = kf_max - 1;

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

w_range::w_range( float start, float end )
	: start( start ), end( end )
{
}

w_range::w_range( std::string_view str )
{
	w_tokenizer tok( str, ',' );

	start = w_parser::float_from_str( *tok.get_next_token() );
	end = w_parser::float_from_str( *tok.get_next_token() );
}

float w_range::clamp_value( float value )
{
	return std::clamp( value, start, end );
}

float w_range::get_value()
{
	return start + ( ( end - start ) * engine->random->getf() );
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

w_rect::w_rect( const w_vec2& top_left, const w_vec2& bottom_right )
	: x( top_left.x ), y( top_left.y ), w( bottom_right.x - top_left.x ), h( bottom_right.y - top_left.y )
{
}

w_vec2 w_rect::extents() const
{
	return { ( w - x ), ( h - y ) };
}

w_vec2 w_rect::top_left() const
{
	return { x, y };
}

w_vec2 w_rect::bottom_right() const
{
	return { x + w, y + h };
}

w_vec2 w_rect::midpoint() const
{
	return w_vec2( x + ( w / 2.0f ), y + ( h / 2.0f ) );
}

// returns a position within this rectangle that represents
// the best spot to start drawing from, given the alignment
// requested.

w_pos w_rect::get_position_from_alignment( e_align align ) const
{
	w_pos pos = { x, y };

	if( align & align::right )
	{
		pos.x = x + w;
	}
	if( align & align::hcenter )
	{
		pos.x = x + ( w / 2.0f );
	}

	if( align & align::bottom )
	{
		pos.y = y + h;
	}
	if( align & align::vcenter )
	{
		pos.y = y + ( h / 2.0f );
	}

	return pos;
}

bool w_rect::operator==( const w_rect& rhs ) const
{
	return( fequals( rhs.x, x ) && fequals( rhs.y, y ) && fequals( rhs.w, w ) && fequals( rhs.h, h ) );
}

w_rect w_rect::operator+( const w_vec2& v ) const
{
	return w_rect( this->x + v.x, this->y + v.y, this->w, this->h );
}

w_rect w_rect::operator+=( const w_vec2& v )
{
	*this = *this + v;
	return *this;
}

w_rect w_rect::operator+( const w_rect& rhs ) const
{
	return w_rect( x + rhs.x, y + rhs.y, w + rhs.w, h + rhs.h );
}

w_rect w_rect::operator-( const w_rect& rhs ) const
{
	return w_rect( x - rhs.x, y - rhs.y, w - rhs.w, h - rhs.h );
}

w_rect w_rect::operator*( float v ) const
{
	return w_rect( this->x * v, this->y * v, this->w * v, this->h * v );
}

w_rect w_rect::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

c2AABB w_rect::as_c2AABB()
{
	c2AABB bb = {};
	bb.min = { x, y };
	bb.max = { x + w, y + h };

	return bb;
}

// ----------------------------------------------------------------------------

w_color w_color::white = w_color( 1.0f, 1.0f, 1.0f );
w_color w_color::black = w_color( 0.0f, 0.0f, 0.0f );
w_color w_color::red = w_color( 1.0f, 0.0f, 0.0f );
w_color w_color::green = w_color( 0.0f, 1.0f, 0.0f );
w_color w_color::blue = w_color( 0.0f, 0.0f, 1.0f );
w_color w_color::light_blue = w_color( 0.5f, 0.5f, 1.0f );
w_color w_color::orange = w_color( 1.0f, 0.5f, 0.0f );
w_color w_color::yellow = w_color( 1.0f, 1.0f, 0.0f );
w_color w_color::teal = w_color( 0.3f, 0.8f, 1.0f );
w_color w_color::dark_teal = w_color( 0.2f, 0.4f, 0.5f );
w_color w_color::dark_grey = w_color( 0.25f, 0.25f, 0.25f );
w_color w_color::grey = w_color( 0.5f, 0.5f, 0.5f );
w_color w_color::light_grey = w_color( 0.75f, 0.75f, 0.75f );
w_color w_color::light_green = w_color( 0.5f, 1.0f, 0.5f );
w_color w_color::dark_green = w_color( 0.25f, 0.5f, 0.25f );
w_color w_color::magenta = w_color( 0.96f, 0.32f, 0.65f );

w_color::w_color( float r, float g, float b, float a )
	: r( r ), g( g ), b( b ), a( a )
{
}

// integer values are assumed to be in the 0-255 range and are converted to float
w_color::w_color( int r, int g, int b, int a )
	: w_color( r* byte_color_to_float, g* byte_color_to_float, b* byte_color_to_float, a* byte_color_to_float )
{
}

// strings can contain any supported kind of data format (hex, int, or floats)
w_color::w_color( const std::string& str )
{
	assert( !str.empty() );

	if( str[ 0 ] == '@' )
	{
		// strings starting with a '@' char are palette indices
		w_tokenizer tok( str, '@' );
		int idx = str_to_int( std::string( tok.get_next_token().value_or( "0" ) ) );
		*this = w_color::pal( idx );
	}
	else if( str[ 0 ] == '$' )
	{
		// strings starting with a '$' char are hex values
		assert( str.length() == 7 );

		r = str_to_uint( "$" + str.substr( 1, 2 ) ) * byte_color_to_float;
		g = str_to_uint( "$" + str.substr( 3, 2 ) ) * byte_color_to_float;
		b = str_to_uint( "$" + str.substr( 5, 2 ) ) * byte_color_to_float;
	}
	else
	{
		// in case this is a set of values, remove the surrounding braces.
		auto new_str = w_string_util::remove_char( str, '[' );
		new_str = w_string_util::remove_char( new_str, ']' );

		w_tokenizer tok( new_str, ',' );

		r = w_parser::float_from_str( *tok.get_next_token() );
		g = w_parser::float_from_str( *tok.get_next_token() );
		b = w_parser::float_from_str( *tok.get_next_token() );
		a = w_parser::float_from_str( tok.get_next_token().value_or( "1.0f" ) );

		// if the colors values are greater than 1.0, they are assumed to be
		// in 0-255 space and are converted back down to 0-1.

		if( r > 1.0f || g > 1.0f || b > 1.0f || a > 1.0f )
		{
			r *= byte_color_to_float;
			g *= byte_color_to_float;
			b *= byte_color_to_float;
			a *= byte_color_to_float;
		}
	}
}

w_color w_color::operator*( float v ) const
{
	return w_color( this->r * v, this->g * v, this->b * v );
}

w_color w_color::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

void w_color::scale( w_color& color, float s )
{
	color.r *= s;
	color.g *= s;
	color.b *= s;
}

w_color w_color::pal( int idx )
{
	return RENDER->get_palette_color_from_idx( idx );
}

// ----------------------------------------------------------------------------

const w_vec2 w_vec2::zero = w_vec2( 0.0f, 0.0f );
const w_vec2 w_vec2::left = w_vec2( -1.0f, 0.0f );
const w_vec2 w_vec2::right = w_vec2( 1.0f, 0.0f );
const w_vec2 w_vec2::up = w_vec2( 0.0f, -1.0f );
const w_vec2 w_vec2::down = w_vec2( 0.0f, 1.0f );
const float w_vec2::def = -1.0f;
const float w_vec2::ignore = -2.0f;

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

w_vec2::w_vec2( const b2Vec2& b2v2 )
	: x( b2v2.x ), y( b2v2.y )
{
}

b2Vec2 w_vec2::as_b2Vec2()
{
	return b2Vec2( x, y );
}

c2v w_vec2::as_c2v()
{
	c2v v = { x, y };
	return v;
}

w_vec2 w_vec2::to_b2d() const
{
	return w_vec2( ::to_b2d( x ), ::to_b2d( y ) );
}

w_vec2 w_vec2::from_b2d() const
{
	return w_vec2( ::from_b2d( x ), ::from_b2d( y ) );
}

bool w_vec2::operator==( const w_vec2& v ) const
{
	return ( fequals( this->x, v.x ) && fequals( this->y, v.y ) );
}

bool w_vec2::operator!=( const w_vec2& v ) const
{
	return !( fequals( this->x, v.x ) && fequals( this->y, v.y ) );
}

w_vec2 w_vec2::operator+( const w_vec2& v ) const
{
	return w_vec2( this->x + v.x, this->y + v.y );
}

w_vec2 w_vec2::operator+=( const w_vec2& v )
{
	*this = *this + v;
	return *this;
}

w_vec2 w_vec2::operator-( const w_vec2& v ) const
{
	return w_vec2( this->x - v.x, this->y - v.y );
}

w_vec2 w_vec2::operator-=( const w_vec2& v )
{
	*this = *this - v;
	return *this;
}

w_vec2 w_vec2::operator*( float v ) const
{
	return w_vec2( this->x * v, this->y * v );
}

w_vec2 w_vec2::operator*=( float v )
{
	*this = *this * v;
	return *this;
}

w_vec2 w_vec2::operator/( float v ) const
{
	return w_vec2( this->x / v, this->y / v );
}

w_vec2 w_vec2::operator/=( float v )
{
	*this = *this / v;
	return *this;
}

float w_vec2::get_size_squared()
{
	return glm::sqrt( w_vec2::get_size_fast() );
}

float w_vec2::get_size_fast()
{
	return ( x * x ) + ( y * y );
}

bool w_vec2::is_zero()
{
	return fequals( x + y, 0.0f );
}

float w_vec2::get_distance_between( const w_vec2& a, const w_vec2& b )
{
	w_vec2 wk = a;
	wk -= b;
	return wk.get_size_squared();
}

// normalizes the vec2 in place, and returns itself in case the caller
// wants to handle it as a return value instead

w_vec2 w_vec2::normalize()
{
	*this /= get_size_squared();
	return *this;
}

// takes an angle, in degrees, and returns a unit vector for it

w_vec2 w_vec2::dir_from_angle( float angle )
{
	w_vec2 v;

	float rad = glm::radians( angle );

	v.x = glm::sin( rad );
	v.y = -glm::cos( rad );

	return v;
}

float w_vec2::angle_from_dir( const w_vec2& dir )
{
	float angle = glm::atan( dir.y, dir.x ) * 180.0f / W_PI;

	angle = glm::mod<float>( angle + 90.f, 360.f );

	return angle;
}

// computes the reflection angle of "v" across the normal "n"

w_vec2 w_vec2::reflect_across_normal( const w_vec2& v, const w_vec2& n )
{
	glm::vec3 rdir = glm::reflect( glm::vec3( v.x, v.y, 0.0f ), glm::vec3( n.x, n.y, 0.0f ) );
	return w_vec2( rdir.x, rdir.y );
}

// ----------------------------------------------------------------------------

const w_vec3 w_vec3::zero = w_vec3( 0.0f, 0.0f, 0.0f );

w_vec3::w_vec3()
	: x( 0.0f ), y( 0.0f ), z( 0.0f )
{
}

w_vec3::w_vec3( int x, int y, int z )
	: x( static_cast<float>( x ) ), y( static_cast<float>( y ) ), z( static_cast<float>( z ) )
{
}

w_vec3::w_vec3( float x, float y, float z )
	: x( x ), y( y ), z( z )
{
}

w_vec3::w_vec3( std::string_view str )
{
	w_tokenizer tok( str, ',' );

	x = w_parser::float_from_str( *tok.get_next_token() );
	y = w_parser::float_from_str( *tok.get_next_token() );
	z = w_parser::float_from_str( *tok.get_next_token() );
}

// ----------------------------------------------------------------------------

w_bbox::w_bbox()
{
	reset();
}

void w_bbox::add( const w_vec2& vtx )
{
	min.x = glm::min( min.x, vtx.x );
	min.y = glm::min( min.y, vtx.y );

	max.x = glm::max( max.x, vtx.x );
	max.y = glm::max( max.y, vtx.y );
}

void w_bbox::reset()
{
	min.x = min.y = std::numeric_limits<float>::max();
	max.x = max.y = std::numeric_limits<float>::min();
}

// ----------------------------------------------------------------------------

w_transform::w_transform()
{
	matrix.set_identity();
}

w_transform* w_transform::set( w_vec2 pos, float angle, float scale )
{
	set_position( pos );
	set_angle( angle );
	set_scale( scale );

	return this;
}

w_transform* w_transform::set_position( w_vec2 pos )
{
	this->pos.x = snap_to_pixel( pos.x );
	this->pos.y = snap_to_pixel( pos.y );

	rebuild_tform();

	return this;
}

w_transform* w_transform::set_angle( float angle )
{
	this->angle = snap_to_pixel( angle );

	rebuild_tform();

	return this;
}

w_transform* w_transform::set_scale( float scale )
{
	this->scale = scale;

	rebuild_tform();

	return this;
}

// compiles the current transform into a matrix for ease of use

void w_transform::rebuild_tform()
{
	matrix.set_identity();
	matrix.translate( pos );
	matrix.rotate( angle );
	matrix.scale( scale );
}
