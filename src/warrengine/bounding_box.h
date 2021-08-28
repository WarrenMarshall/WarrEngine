
namespace war
{

struct Bounding_Box final
{
	Vec2 min, max;

	Bounding_Box();

	void add( const Vec2& vtx );
	void reset();

	Bounding_Box operator+( const Vec2& v ) const;
	Bounding_Box operator+=( const Vec2& v );
	Bounding_Box operator+( const Bounding_Box& bbox ) const;
	Bounding_Box operator+=( const Bounding_Box& bbox );
};

}
