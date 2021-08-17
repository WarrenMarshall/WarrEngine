
namespace war
{

struct color
{
	float r = 0.f, g = 0.f, b = 0.f, a = 1.f;

	static const color white;
	static const color black;
	static const color dark_red;
	static const color red;
	static const color dark_green;
	static const color green;
	static const color light_green;
	static const color dark_blue;
	static const color blue;
	static const color light_blue;
	static const color orange;
	static const color yellow;
	static const color dark_teal;
	static const color teal;
	static const color dark_grey;
	static const color grey;
	static const color light_grey;
	static const color magenta;

	color() = default;
	color( float r, float g, float b, float a = 1.f );
	color( int r, int g, int b, int a = 255 );
	color( std::string& str );

	color operator*( float v ) const;
	color operator*=( float v );
	color operator-( color v ) const;
	color operator-=( color v );
	color operator+( color v ) const;
	color operator+=( color v );

	static void scale( color& color, float s );

	static color make( const color& clr, float alpha = 1.f );
	static color make( int pal_idx, float alpha = 1.f );
};

}
