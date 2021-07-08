
namespace war
{

struct color
{
	float r = 0.f, g = 0.f, b = 0.f, a = 1.f;

	static const color white;
	static const color black;
	static const color dark_red;
	static const color red;
	static const color dark_green;
	static const color green;
	static const color blue;
	static const color light_blue;
	static const color orange;
	static const color yellow;
	static const color dark_teal;
	static const color teal;
	static const color dark_grey;
	static const color grey;
	static const color light_grey;
	static const color light_green;
	static const color magenta;

	color() = default;
	color( float r, float g, float b, float a = 1.f );
	color( int r, int g, int b, int a = 255 );
	color( std::string& str );

	color operator*( float v ) const;
	color operator*=( float v );
	color operator-( color v ) const;
	color operator-=( color v );
	color operator+( color v ) const;
	color operator+=( color v );

	static void scale( color& color, float s );

	static color make( const color& clr, float alpha = 1.f );
	static color make( int pal_idx, float alpha = 1.f );
};

// ----------------------------------------------------------------------------
// a single stop on a timeline

struct timeline_key_frame
{
	float pct_marker = 0.f;
	std::variant<float, color> value = {};

	timeline_key_frame() = default;
	timeline_key_frame( float pct_marker, float value );
	timeline_key_frame( float pct_marker, const color& value );
};

// represents successive steps through a 0-1 timeline

struct timeline
{
	std::vector<timeline_key_frame> key_frames;
	e_timeline_type type = timeline_type::invalid;

	timeline( e_timeline_type type );

	timeline* clear_key_frames();
	timeline* add_key_frame( const timeline_key_frame& kf );

	size_t find_next_key_frame_idx_from_pct( float pct );

	// computes a value on the timeline between 0-1, based on the
	// "pct_on_timeline" pass in.

	template<typename T>
	void get_value( float pct_on_timeline, T* value )
	{
		auto kf_max = find_next_key_frame_idx_from_pct( pct_on_timeline );
		size_t kf_min = kf_max - 1;

		// the range of percentages within the min/max key frames
		float pct_range = key_frames[ kf_max ].pct_marker - key_frames[ kf_min ].pct_marker;

		// the pct we are at within the min/max key frames
		float pct_within = ( pct_on_timeline - key_frames[ kf_min ].pct_marker ) / pct_range;

		// compute the value represented by that pct_within within the min/max key frames

		T min_value = std::get<T>( key_frames[ kf_min ].value );
		T max_value = std::get<T>( key_frames[ kf_max ].value );

		*value = min_value + ( ( max_value - min_value ) * pct_within );
	}
};

// ----------------------------------------------------------------------------

template<typename T>
struct range
{
	T start = {};
	T end = {};

	range() = default;
	range( T start, T end )
		: start( start ), end( end )
	{
	}

	range( std::string_view str )
	{
		tokenizer tok( str, "," );

		start = text_parser::float_from_str( *tok.get_next_token() );
		end = text_parser::float_from_str( *tok.get_next_token() );
	}

	float clamp_value( T value )
	{
		return glm::clamp( value, start, end );
	}

	[[nodiscard]] T get_random_value()
	{
		float rand = random::getf_range( 0.f, 1.f );
		return start + ( ( end - start ) * rand );
	}

	[[nodiscard]] T get_value_at_pct( float pct )
	{
		pct = glm::clamp( pct, 0.f, 1.f );
		return start + ( ( end - start ) * pct );
	}

	[[nodiscard]] T get_pct_from_value( T value )
	{
		float pct = ( value - start ) / ( end - start );

		return glm::clamp( pct, 0.f, 1.f );
	}

	[[nodiscard]] bool is_within( T val )
	{
		return ( val >= start and val <= end );
	}
};

// ----------------------------------------------------------------------------

struct rect
{
	float x = 0.f, y = 0.f, w = 0.f, h = 0.f;

	static const rect zero;

	rect() = default;
	rect( float x, float y );
	rect( float x, float y, float w, float h );
	rect( int x, int y, int w, int h );
	rect( int16 x, int16 y, int16 w, int16 h );
	rect( const vec2& top_left, const vec2& bottom_right );

	vec2 extents() const;
	vec2 top_left() const;
	vec2 bottom_right() const;
	vec2 get_midpoint() const;
	vec2 get_pos_from_alignment( e_align align ) const;
	rect grow( float val );
	rect shrink( float val );
	bool contains_point( vec2 pos );

	rect cut_left( float sz );
	rect cut_right( float sz );
	rect cut_top( float sz );
	rect cut_bottom( float sz );
	rect cut();

	bool operator==( const rect& rhs ) const;
	rect operator+( const vec2& v ) const;
	rect operator+=( const vec2& v );
	rect operator-( const vec2& v ) const;
	rect operator-=( const vec2& v );
	rect operator+( const rect& rhs ) const;
	rect operator-( const rect& rhs ) const;
	rect operator*( float v ) const;
	rect operator*=( float v );

	c2AABB to_c2AABB() const;

	static rect create_centered( float sz );
	static rect create_centered( float w, float h );
};

// ----------------------------------------------------------------------------

struct vec2
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

	static const vec2 zero;
	static const vec2 left;
	static const vec2 right;
	static const vec2 up;
	static const vec2 down;
	static const float defaulted;
	static const float ignored;

	vec2();
	vec2( int x, int y );
	vec2( float x, float y );
	vec2( float v );
	vec2( std::string_view str );
	vec2( const b2Vec2& b2v2 );

	[[nodiscard]] static vec2 compute_uv_tiling( const texture_asset* texture, const rect& rc );
	vec2 normalize();

	[[nodiscard]] b2Vec2 to_b2Vec2() const;
	[[nodiscard]] c2v to_c2v() const;
	[[nodiscard]] vec2 to_box2d() const;
	[[nodiscard]] vec2 from_box2d() const;
	[[nodiscard]] float get_size_squared() const;
	[[nodiscard]] float get_size_fast() const;
	[[nodiscard]] bool is_zero() const;

	[[nodiscard]] bool operator==( const vec2& v ) const;
	[[nodiscard]] bool operator!=( const vec2& v ) const;
	[[nodiscard]] vec2 operator+( const vec2& v ) const;
	[[nodiscard]] vec2 operator-( const vec2& v ) const;
	[[nodiscard]] vec2 operator-() const;
	[[nodiscard]] vec2 operator*( float v ) const;
	[[nodiscard]] vec2 operator*( vec2 v ) const;
	[[nodiscard]] vec2 operator/( float v ) const;

	vec2 operator+=( const vec2& v );
	vec2 operator-=( const vec2& v );
	vec2 operator*=( float v );
	vec2 operator*=( vec2 v );
	vec2 operator/=( float v );
	void operator=( const float& v );

	[[nodiscard]] static float get_distance_between( const vec2& a, const vec2& b );
	[[nodiscard]] static vec2 dir_from_angle( float angle );
	[[nodiscard]] static float angle_from_dir( const vec2& dir );
	[[nodiscard]] static float clamped_angle_from_dir( const vec2& dir );
	[[nodiscard]] static vec2 reflect_across_normal( const vec2& v, const vec2& n );
	[[nodiscard]] static vec2 snap_to_int( const vec2& v );
};

// ----------------------------------------------------------------------------

struct vec3
{
	float x, y, z;

	static const vec3 zero;

	vec3();
	vec3( int x, int y, int z );
	vec3( float x, float y, float z );
	vec3( std::string_view str );
};

// ----------------------------------------------------------------------------

struct bounding_box
{
	vec2 min, max;

	bounding_box();

	void add( const vec2& vtx );
	void reset();

	bounding_box operator+( const vec2& v ) const;
	bounding_box operator+=( const vec2& v );
	bounding_box operator+( const bounding_box& bbox ) const;
	bounding_box operator+=( const bounding_box& bbox );
};

}
