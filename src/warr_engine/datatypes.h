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
	w_color( std::string& str );

	w_color operator*( float v ) const;
	w_color operator*=( float v );
	w_color operator-( w_color v ) const;
	w_color operator-=( w_color v );
	w_color operator+( w_color v ) const;
	w_color operator+=( w_color v );

	static void scale( w_color& color, float s );
	static w_color pal( int idx );
};

// ----------------------------------------------------------------------------
// a single stop on a timeline

struct w_keyframe
{
	float pct_marker = 0.0f;
	w_keyframe_data value = {};

	w_keyframe() = default;
	w_keyframe( float pct_marker, float value );
	w_keyframe( float pct_marker, const w_color& value );
};

// represents successive steps through a 0-1 timeline

struct w_timeline
{
	std::vector<w_keyframe> keyframes;
	e_timeline_type type = timeline_type::invalid;

	w_timeline( e_timeline_type type );

	w_timeline* kf_clear();
	w_timeline* kf_add( const w_keyframe& keyframe );

	size_t find_next_keyframe_idx_from_pct( float pct );

	// computes a value on the timeline between 0-1, based on the
	// "pct_on_timeline" pass in.

	template<typename T>
	void get_value( float pct_on_timeline, T* value )
	{
		auto kf_max = find_next_keyframe_idx_from_pct( pct_on_timeline );
		size_t kf_min = kf_max - 1;

		// the range of percentages within the min/max keyframes
		float pct_range = keyframes[ kf_max ].pct_marker - keyframes[ kf_min ].pct_marker;

		// the pct we are at within the min/max keyframes
		float pct_within = ( pct_on_timeline - keyframes[ kf_min ].pct_marker ) / pct_range;

		// compute the value represented by that pct_within within the min/max keyframes

		T min_value = std::get<T>( keyframes[ kf_min ].value );
		T max_value = std::get<T>( keyframes[ kf_max ].value );

		*value = min_value + ( ( max_value - min_value ) * pct_within );
	}
};

// ----------------------------------------------------------------------------

struct w_range
{
	float start = 0.0f;
	float end = 1.0f;

	w_range() = default;
	w_range( float start, float end );
	w_range( std::string_view str );

	float clamp_value( float value );

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
	w_rect( int x, int y, int w, int h );
	w_rect( short x, short y, short w, short h );
	w_rect( const w_vec2& top_left, const w_vec2& bottom_right );

	w_vec2 extents() const;
	w_vec2 top_left() const;
	w_vec2 bottom_right() const;
	w_vec2 midpoint() const;
	w_pos get_pos_from_alignment( e_align align ) const;

	bool operator==( const w_rect& rhs ) const;
	w_rect operator+( const w_vec2& v ) const;
	w_rect operator+( const w_rect& rhs ) const;
	w_rect operator-( const w_rect& rhs ) const;
	w_rect operator+=( const w_vec2& v );
	w_rect operator*( float v ) const;
	w_rect operator*=( float v );

	c2AABB as_c2AABB() const;
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
	static const float def;
	static const float ignore;

	w_vec2();
	w_vec2( int x, int y );
	w_vec2( float x, float y );
	w_vec2( float v );
	w_vec2( std::string_view str );
	w_vec2( const b2Vec2& b2v2 );

	static w_vec2 compute_uv_tiling( const a_texture* texture, const w_rect& rc );
	w_vec2 normalize();

	_NODISCARD b2Vec2 as_b2Vec2() const;
	_NODISCARD c2v as_c2v() const;
	_NODISCARD w_vec2 to_b2d() const;
	_NODISCARD w_vec2 from_b2d() const;
	_NODISCARD float get_size_squared();
	_NODISCARD float get_size_fast();
	_NODISCARD bool is_zero();

	_NODISCARD bool operator==( const w_vec2& v ) const;
	_NODISCARD bool operator!=( const w_vec2& v ) const;
	_NODISCARD w_vec2 operator+( const w_vec2& v ) const;
	_NODISCARD w_vec2 operator-( const w_vec2& v ) const;
	_NODISCARD w_vec2 operator*( float v ) const;
	_NODISCARD w_vec2 operator/( float v ) const;

	w_vec2 operator+=( const w_vec2& v );
	w_vec2 operator-=( const w_vec2& v );
	w_vec2 operator*=( float v );
	w_vec2 operator/=( float v );
	void operator=( const float& v );

	_NODISCARD static float get_distance_between( const w_vec2& a, const w_vec2& b );
	_NODISCARD static w_vec2 dir_from_angle( float angle );
	_NODISCARD static float angle_from_dir( const w_vec2& dir );
	_NODISCARD static w_vec2 reflect_across_normal( const w_vec2& v, const w_vec2& n );

	w_vec2 snap_to_pixel();
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
