
namespace war
{

struct Bounding_Box
{
	Vec2 min, max;

	Bounding_Box();
	Bounding_Box( const Vec2& v1, const Vec2& v2 );

	void add( const Vec2& vtx );
	void reset();
	[[nodiscard]] Vec2 get_random_spot() const;

	Bounding_Box operator+( const Vec2& v ) const;
	Bounding_Box operator+=( const Vec2& v );
	Bounding_Box operator+( const Bounding_Box& bbox ) const;
	Bounding_Box operator+=( const Bounding_Box& bbox );
};

}
