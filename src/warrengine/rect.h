
namespace war
{

struct Rect
{
	float x = 0.f, y = 0.f, w = 0.f, h = 0.f;

	static const Rect zero;

	Rect() = default;
	Rect( float x, float y );
	Rect( float x, float y, float w, float h );
	Rect( int x, int y, int w, int h );
	Rect( int16 x, int16 y, int16 w, int16 h );
	Rect( const Vec2& top_left, const Vec2& bottom_right );

	Vec2 extents() const;
	Vec2 top_left() const;
	Vec2 bottom_right() const;
	Vec2 get_midpoint() const;
	Vec2 get_pos_from_alignment( e_align_t align ) const;
	Rect grow( float val );
	Rect shrink( float val );
	bool contains_point( Vec2 pos );

	Rect cut_left( float sz );
	Rect cut_right( float sz );
	Rect cut_top( float sz );
	Rect cut_bottom( float sz );
	Rect cut();

	bool operator==( const Rect& rhs ) const;
	Rect operator+( const Vec2& v ) const;
	Rect operator+=( const Vec2& v );
	Rect operator-( const Vec2& v ) const;
	Rect operator-=( const Vec2& v );
	Rect operator+( const Rect& rhs ) const;
	Rect operator-( const Rect& rhs ) const;
	Rect operator*( float v ) const;
	Rect operator*=( float v );

	c2AABB to_c2AABB() const;

	static Rect create_centered( float sz );
	static Rect create_centered( float w, float h );
};

}
