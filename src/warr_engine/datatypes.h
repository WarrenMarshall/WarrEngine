#pragma once

// ----------------------------------------------------------------------------

struct w_color
{
	float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

	static w_color white;
	static w_color black;
	static w_color red;
	static w_color green;
	static w_color blue;
	static w_color light_blue;
	static w_color orange;
	static w_color yellow;
	static w_color teal;
	static w_color dark_teal;
	static w_color dark_grey;
	static w_color grey;
	static w_color light_grey;
	static w_color light_green;
	static w_color dark_green;
	static w_color magenta;

	w_color() = default;
	w_color( float r, float g, float b, float a = 1.0f );
	w_color( int r, int g, int b, int a = 255 );
	w_color( const std::string& str );

	w_color operator*( float v ) const;
	w_color operator*=( float v );

	static void scale( w_color& color, float s );
	static w_color pal( int idx );
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
	w_keyframe( float pct_marker, float value );
	w_keyframe( float pct_marker, const w_color& value );
};

/*
	represents successive steps through a 0-1 timeline
*/

struct w_timeline
{
	std::vector<w_keyframe> keyframes;
	e_timeline_type type = timeline_type::invalid;

	w_timeline( e_timeline_type type );

	w_timeline* kf_clear();
	w_timeline* kf_add( const w_keyframe& keyframe );

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

	[[nodiscard]] float get_value();
};

// ----------------------------------------------------------------------------

struct w_rect
{
	float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;

	static const w_rect zero;

	w_rect() = default;
	w_rect( float x, float y );
	w_rect( float x, float y, float w, float h );
	w_rect( const w_vec2& top_left, const w_vec2& bottom_right );

	float left()
	{
		return x;
	}
	float top()
	{
		return y;
	}
	float bottom()
	{
		return y + h;
	}
	float right()
	{
		return x + w;
	}

	w_vec2 midpoint();

	bool operator==( const w_rect& rhs ) const;
	w_rect operator+( const w_vec2& v ) const;
	w_rect operator+=( const w_vec2& v );
	w_rect operator*( float v ) const;
	w_rect operator*=( float v );

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
			float w, h;		// width/height
		};
		struct
		{
			float l, r;		// left/right
		};
		struct
		{
			float t, b;		// top/bottom
		};
	};

	static const w_vec2 zero;
	static const w_vec2 left;
	static const w_vec2 right;
	static const w_vec2 up;
	static const w_vec2 down;

	w_vec2();
	w_vec2( int x, int y );
	w_vec2( float x, float y );
	w_vec2( std::string_view str );
	w_vec2( const b2Vec2& b2v2 );

	w_vec2 normalize();
	[[nodiscard]] b2Vec2 as_b2Vec2();
	[[nodiscard]] c2v as_c2v();
	[[nodiscard]] w_vec2 to_b2d() const;
	[[nodiscard]] w_vec2 from_b2d() const;
	[[nodiscard]] float get_size_squared();
	[[nodiscard]] float get_size_fast();
	[[nodiscard]] bool is_zero();

	[[nodiscard]] bool operator==( const w_vec2& v ) const;
	[[nodiscard]] bool operator!=( const w_vec2& v ) const;
	[[nodiscard]] w_vec2 operator+( const w_vec2& v ) const;
	[[nodiscard]] w_vec2 operator-( const w_vec2& v ) const;
	[[nodiscard]] w_vec2 operator*( float v ) const;
	[[nodiscard]] w_vec2 operator/( float v ) const;

	w_vec2 operator+=( const w_vec2& v );
	w_vec2 operator-=( const w_vec2& v );
	w_vec2 operator*=( float v );
	w_vec2 operator/=( float v );

	[[nodiscard]] static float get_distance_between( const w_vec2& a, const w_vec2& b );
	[[nodiscard]] static w_vec2 dir_from_angle( float angle );
	[[nodiscard]] static float angle_from_dir( const w_vec2& dir );
	[[nodiscard]] static w_vec2 reflect_across_normal( const w_vec2& v, const w_vec2& n );
};

// ----------------------------------------------------------------------------

struct w_vec3
{
	float x, y, z;

	static const w_vec3 zero;

	w_vec3();
	w_vec3( int x, int y, int z );
	w_vec3( float x, float y, float z );
	w_vec3( std::string_view str );
};

// ----------------------------------------------------------------------------

struct w_bbox
{
	w_vec2 min, max;

	w_bbox();

	void add( const w_vec2& vtx );
	void reset();
};

// ----------------------------------------------------------------------------

struct w_transform
{
	// position
	w_vec2 pos = w_vec2( 0, 0 );

	// rotation (around z axis)
	float angle = 0.0f;

	// scale
	float scale = 1.0f;

	w_transform();

	void set( w_vec2 pos, float angle, float scale );
	void set_pos( w_vec2 pos );
	void set_angle( float angle );
	void set_scale( float scale );

	void rebuild_tform();

	w_matrix matrix;
};
