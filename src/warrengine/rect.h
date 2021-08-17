
namespace war
{

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

}
