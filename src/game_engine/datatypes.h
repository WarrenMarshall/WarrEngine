#pragma once

// ----------------------------------------------------------------------------

struct w_color
{
	float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

	static const w_color white;
	static const w_color black;
	static const w_color red;
	static const w_color green;
	static const w_color blue;
	static const w_color light_blue;
	static const w_color orange;
	static const w_color yellow;
	static const w_color teal;
	static const w_color dark_teal;
	static const w_color dark_grey;
	static const w_color grey;
	static const w_color light_grey;
	static const w_color light_green;
	static const w_color dark_green;
	static const w_color magenta;

	w_color() = default;
	w_color( const w_color& other );
	w_color( float r, float g, float b, float a = 1.0f );
	w_color( int r, int g, int b, int a = 255 );
	w_color( std::string& str );

	static void scale( w_color& color, float s );
};

// ----------------------------------------------------------------------------

/*
	a single stop on a timeline
*/

struct w_keyframe
{
	float pct_marker = 0.0f;
	float float_value = 0.0f;
	w_color color_value = w_color::white;

	w_keyframe() = default;
	w_keyframe( const w_keyframe& other );
	w_keyframe( float pct_marker, float value );
	w_keyframe( float pct_marker, w_color value );
};

/*
	represents successive steps through a 0-1 timeline
*/

struct w_timeline
{
	std::vector<w_keyframe> keyframes;
	e_timeline_type type = timeline_type::invalid;

	w_timeline( e_timeline_type type );
	w_timeline( const w_timeline& other );

	w_timeline* kf_clear();
	w_timeline* kf_add( w_keyframe keyframe );

	void get_value( float pct_on_timeline, float* value );
	void get_value( float pct_on_timeline, w_color* value );
};

// ----------------------------------------------------------------------------

struct w_range
{
	float start = 0.0f;
	float end = 1.0f;

	w_range() = default;
	w_range( float start, float end );
	w_range( std::string_view str );

	_NODISCARD float get_value();
};

// ----------------------------------------------------------------------------

struct w_rect
{
	float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;

	static const w_rect zero;

	w_rect() = default;
	w_rect( float x, float y );
	w_rect( float x, float y, float w, float h );
	w_rect( w_vec2 top_left, w_vec2 bottom_right );

	constexpr float left()
	{
		return x;
	}
	constexpr float top()
	{
		return y;
	}
	constexpr float bottom()
	{
		return y + h;
	}
	constexpr float right()
	{
		return x + w;
	}

	w_vec2 midpoint();

	w_rect operator+( const w_vec2 v );
	w_rect operator+=( const w_vec2 v );

	c2AABB as_c2AABB();
};

// ----------------------------------------------------------------------------

struct w_vec2
{
	union
	{
		struct
		{
			float x, y;		// xy position
		};

		// these are designed to make code easier to read. so if the w_vec2's values
		// are being used in a specific way, these unioned names can make the
		// intention clearer.

		struct
		{
			float u, v;		// uv coord
		};
		struct
		{
			float _width, _height;
		};
		struct
		{
			float _left, _right;
		};
		struct
		{
			float _top, _bottom;
		};
	};

	static const w_vec2 zero;
	static const w_vec2 left;
	static const w_vec2 right;
	static const w_vec2 up;
	static const w_vec2 down;

	static w_vec2 get_random_unit();

	w_vec2();
	w_vec2( int x, int y );
	w_vec2( float x, float y );
	w_vec2( std::string_view str );
	w_vec2( b2Vec2 b2v2 );

	w_vec2& normalize();
	b2Vec2 as_b2Vec2();
	c2v as_c2v();
	w_vec2 to_b2d();
	w_vec2 from_b2d();
	float get_size_squared();
	float get_size();
	w_vec2& snap_to_pixel();

	bool operator==( w_vec2 v );
	bool operator!=( w_vec2 v );
	w_vec2 operator+( const w_vec2 v );
	w_vec2 operator+=( w_vec2 v );
	w_vec2 operator-( w_vec2 v );
	w_vec2 operator-=( w_vec2 v );
	w_vec2 operator*( float v );
	w_vec2 operator*=( float v );
	w_vec2 operator/( float v );
	w_vec2 operator/=( float v );

	_NODISCARD static float get_distance_between( w_vec2 a, w_vec2 b );
	static w_vec2 normalize( w_vec2 a );
	_NODISCARD static w_vec2 dir_from_angle( float angle );
	_NODISCARD static float angle_from_dir( w_vec2 dir );
	_NODISCARD static w_vec2 reflect_across_normal( w_vec2 v, w_vec2 n );
};

// ----------------------------------------------------------------------------

struct w_bbox
{
	w_vec2 min, max;

	w_bbox();

	void add( w_vec2 vtx );
	void reset();
};
