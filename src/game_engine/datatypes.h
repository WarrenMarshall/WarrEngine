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
	w_color( std::string_view str );

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
	e_timeline_type type = timeline_type::invalid;
	std::vector<w_keyframe> keyframes;

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
	float min = 0.0f;
	float max = 1.0f;

	w_range() = default;
	w_range( float min, float max );
	w_range( std::string_view str );

	float get_value();
};

// ----------------------------------------------------------------------------

struct w_rect
{
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;

	w_rect() = default;
	w_rect( float x, float y, float w = -1.0f, float h = -1.0f );

	operator c2AABB()
	{
		c2AABB bb;
		bb.min = { x, y };
		bb.max = { x + w, y + h };

		return bb;
	}
};

// ----------------------------------------------------------------------------

struct w_vec2
{
	union
	{
		struct
		{
			float x, y;
		};
		struct
		{
			float u, v;
		};
		struct
		{
			float w, h;
		};
	};

	static const w_vec2 zero;

	w_vec2();
	w_vec2( int x, int y );
	w_vec2( float x, float y );
	w_vec2( std::string_view str );

	w_vec2 operator+( const w_vec2& v );
	w_vec2 operator-( const w_vec2& v );
	w_vec2 operator=( const w_vec2& v );
	w_vec2 operator*( const float& f );

	static w_vec2 scale( w_vec2 a, float scale );
	static float get_size_squared( w_vec2 a );
	static float get_size( w_vec2 a );
	static float get_distance_between( w_vec2 a, w_vec2 b );
	static w_vec2 normalize( w_vec2 a );
	static w_vec2 from_angle( float angle );

	operator c2v()
	{
		c2v v;
		v.x = x;
		v.y = y;

		return v;
	}
};

// ----------------------------------------------------------------------------

struct w_vec3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	static const w_vec3 zero;

	w_vec3();
	w_vec3( const w_vec3& v );
	w_vec3( float x, float y, float z );
	w_vec3( int x, int y, int z );
	w_vec3( std::string_view str );

	w_vec3 operator+( const w_vec3& v );
	w_vec3 operator-( const w_vec3& v );
	w_vec3 operator=( const w_vec3& v );
	w_vec3 operator*( const float& f);

	static float get_size_squared( w_vec3 a );
	static float get_size( w_vec3 a );
	static float get_distance_between( w_vec3 a, w_vec3 b );
	static w_vec3 normalize( w_vec3 a );
};

// ----------------------------------------------------------------------------

struct w_bbox
{
	w_vec2 min, max;

	w_bbox();

	void add( w_vec2 vtx );
	void reset();
};