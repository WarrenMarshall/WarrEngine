
namespace war
{

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
