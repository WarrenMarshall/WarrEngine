
namespace war
{

struct Bounding_Box final
{
	Vec2 min, max;

	Bounding_Box();
	Bounding_Box( const Vec2& v1, const Vec2& v2 );

	void add( const Vec2& vtx );
	void reset();
	[[nodiscard]] Vec2 get_random_spot() const;

	[[nodiscard]] Rect as_rect() const;

	Bounding_Box operator+( const Vec2& v ) const;
	Bounding_Box operator+=( const Vec2& v );
	Bounding_Box operator+( const Rect& r ) const;
	Bounding_Box operator+=( const Rect& r );
	Bounding_Box operator+( const Bounding_Box& bbox ) const;
	Bounding_Box operator+=( const Bounding_Box& bbox );
};

}
