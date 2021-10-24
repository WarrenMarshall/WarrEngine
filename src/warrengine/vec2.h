
namespace war
{

struct Vec2 final
{
	union
	{
		struct
		{
			f32 x, y;		// xy position
		};

		// these are designed to make code easier to read. so if the w_vec2's values
		// are being used in a specific way, these unioned names can make the
		// intention clearer.

		struct
		{
			f32 u, v;		// uv coord
		};
		struct
		{
			f32 w, h;		// width/height
		};
		struct
		{
			f32 l, r;		// left/right
		};
		struct
		{
			f32 t, b;		// top/bottom
		};
	};

	static const Vec2 zero;
	static const Vec2 left;
	static const Vec2 right;
	static const Vec2 up;
	static const Vec2 down;
	static const Vec2 x_axis;
	static const Vec2 y_axis;
	static const f32 defaulted;
	static const f32 ignored;

	// ctors

	Vec2();
	Vec2( i32 x, i32 y );
	Vec2( f32 x, f32 y );
	Vec2( f32 v );
	Vec2( std::string_view str );
	Vec2( const b2Vec2& b2v2 );
	Vec2 snap_to_int();

	// static utilities

	[[nodiscard]] static Vec2 compute_uv_tiling( const Texture_Asset* texture, const Rect& rc );
	[[nodiscard]] static Vec2 normalize( const Vec2& v );
	[[nodiscard]] static Vec2 angle_to_dir( f32 angle );
	[[nodiscard]] static f32 dir_to_angle( const Vec2& dir );
	[[nodiscard]] static f32 clamped_angle_from_dir( const Vec2& dir );
	[[nodiscard]] static Vec2 reflect_across_normal( const Vec2& v, const Vec2& n );
	[[nodiscard]] static Vec2 snap_to_int( const Vec2& v );
	[[nodiscard]] static f32 dot( const Vec2& a, const Vec2& b );
	[[nodiscard]] static Vec2 cross( const Vec2& a, const Vec2& b );

	// members

	[[nodiscard]] b2Vec2 to_b2Vec2() const;
	[[nodiscard]] c2v to_c2v() const;
	[[nodiscard]] c2v as_c2v() const;
	[[nodiscard]] Vec2 to_box2d() const;
	[[nodiscard]] Vec2 from_box2d() const;
	[[nodiscard]] Vec2 to_simple() const;
	[[nodiscard]] Vec2 from_simple() const;
	[[nodiscard]] f32 get_size() const;
	[[nodiscard]] f32 get_size_fast() const;
	[[nodiscard]] bool is_zero() const;
	[[nodiscard]] bool is_nan() const;
	Vec2 normalize();
	[[nodiscard]] Vec2 normalize() const;
	Vec2 clamp( f32 value );


	// operators

	[[nodiscard]] bool operator==( const Vec2& v ) const;
	[[nodiscard]] bool operator!=( const Vec2& v ) const;
	[[nodiscard]] Vec2 operator+( const Vec2& v ) const;
	[[nodiscard]] Vec2 operator-( const Vec2& v ) const;
	[[nodiscard]] Vec2 operator-() const;
	[[nodiscard]] Vec2 operator*( f32 v ) const;
	[[nodiscard]] Vec2 operator*( Vec2 v ) const;
	[[nodiscard]] Vec2 operator/( f32 v ) const;

	Vec2 operator+=( const Vec2& v );
	Vec2 operator-=( const Vec2& v );
	Vec2 operator*=( f32 v );
	Vec2 operator*=( Vec2 v );
	Vec2 operator/=( f32 v );
	void operator=( const float& v );
	void operator=( const Vec2& v );
};

}
