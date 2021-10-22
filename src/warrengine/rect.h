
namespace war
{

struct Rect final
{
	f32 x = 0.f, y = 0.f, w = 0.f, h = 0.f;

	static const Rect zero;

	Rect() = default;
	Rect( f32 x, f32 y );
	Rect( f32 x, f32 y, f32 w, f32 h );
	Rect( i32 x, i32 y, i32 w, i32 h );
	Rect( const Vec2 & top_left, const Vec2 & width_height );
	Rect( const Vec2 & top_left, f32 width, f32 height );
	Rect( const Vec2 & pos );

	Vec2 extents() const;
	Vec2 top_left() const;
	Vec2 bottom_right() const;
	Vec2 get_midpoint() const;
	Vec2 get_pos_from_alignment( e_align align ) const;
	Rect grow( f32 val );
	Rect shrink( f32 val );
	bool contains_point( Vec2 pos );

	Rect cut_left( f32 sz );
	Rect cut_right( f32 sz );
	Rect cut_top( f32 sz );
	Rect cut_bottom( f32 sz );
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
	Rect operator*( f32 v ) const;
	Rect operator*=( f32 v );

	c2AABB to_c2AABB() const;
	c2AABB as_c2AABB() const;

	static Rect create_centered( f32 sz );
	static Rect create_centered( f32 w, f32 h );
	i32 area();
};

}
