
namespace war
{

struct Rect final
{
	float_t x = 0.f, y = 0.f, w = 0.f, h = 0.f;

	static const Rect zero;

	Rect() = default;
	Rect( float_t x, float_t y );
	Rect( float_t x, float_t y, float_t w, float_t h );
	Rect( int32_t x, int32_t y, int32_t w, int32_t h );
	Rect( const Vec2 & top_left, const Vec2 & width_height );
	Rect( const Vec2 & top_left, float_t width, float_t height );
	Rect( const Vec2 & pos );

	Vec2 extents() const;
	Vec2 top_left() const;
	Vec2 bottom_right() const;
	Vec2 get_midpoint() const;
	Vec2 get_pos_from_alignment( e_align align ) const;
	Rect grow( float_t val );
	Rect shrink( float_t val );
	bool contains_point( Vec2 pos );

	Rect cut_left( float_t sz );
	Rect cut_right( float_t sz );
	Rect cut_top( float_t sz );
	Rect cut_bottom( float_t sz );
	Rect cut();

	std::vector<Rect> subdivide() const;
	Vec2 find_random_pos_within();

	bool operator==( const Rect& rhs ) const;
	Rect operator+( const Vec2 & v ) const;
	Rect operator+=( const Vec2 & v );
	Rect operator-( const Vec2 & v ) const;
	Rect operator-=( const Vec2 & v );
	Rect operator+( const Rect & rhs ) const;
	Rect operator-( const Rect & rhs ) const;
	Rect operator*( float_t v ) const;
	Rect operator*=( float_t v );

	c2AABB to_c2AABB() const;
	c2AABB as_c2AABB() const;

	static Rect create_centered( float_t sz );
	static Rect create_centered( float_t w, float_t h );
	int32_t area();
};

}
