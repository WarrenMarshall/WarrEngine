
namespace war
{

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
	static const vec2 x_axis;
	static const vec2 y_axis;
	static const float defaulted;
	static const float ignored;

	// ctors

	vec2();
	vec2( int x, int y );
	vec2( float x, float y );
	vec2( float v );
	vec2( std::string_view str );
	vec2( const b2Vec2& b2v2 );
	vec2 snap_to_int();

	// static utilities

	[[nodiscard]] static vec2 compute_uv_tiling( const texture_asset* texture, const rect& rc );
	[[nodiscard]] static vec2 normalize( const vec2& v );
	[[nodiscard]] static vec2 dir_from_angle( float angle );
	[[nodiscard]] static float angle_from_dir( const vec2& dir );
	[[nodiscard]] static float clamped_angle_from_dir( const vec2& dir );
	[[nodiscard]] static vec2 reflect_across_normal( const vec2& v, const vec2& n );
	[[nodiscard]] static vec2 snap_to_int( const vec2& v );
	[[nodiscard]] static float dot( const vec2& a, const vec2& b );
	[[nodiscard]] static vec2 cross( const vec2& a, const vec2& b );

	// members

	[[nodiscard]] b2Vec2 to_b2Vec2() const;
	[[nodiscard]] c2v to_c2v() const;
	[[nodiscard]] vec2 to_box2d() const;
	[[nodiscard]] vec2 from_box2d() const;
	[[nodiscard]] vec2 to_simple() const;
	[[nodiscard]] vec2 from_simple() const;
	[[nodiscard]] float get_size() const;
	[[nodiscard]] float get_size_fast() const;
	[[nodiscard]] bool is_zero() const;
	[[nodiscard]] bool is_nan() const;
	[[nodiscard]] vec2 normalize();
	[[nodiscard]] vec2 normalize() const;
	vec2 clamp( float value );


	// operators

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
	void operator=( const vec2& v );
};

}
