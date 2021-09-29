
namespace war
{

struct Color final
{
	float_t r = 0.f, g = 0.f, b = 0.f, a = 1.f;

	static const Color white;
	static const Color black;
	static const Color dark_red;
	static const Color red;
	static const Color dark_green;
	static const Color green;
	static const Color light_green;
	static const Color dark_blue;
	static const Color blue;
	static const Color light_blue;
	static const Color orange;
	static const Color yellow;
	static const Color dark_teal;
	static const Color teal;
	static const Color dark_grey;
	static const Color grey;
	static const Color light_grey;
	static const Color magenta;

	Color() = default;
	Color( float_t r, float_t g, float_t b, float_t a = 1.f );
	Color( float_t v, float_t a = 1.f );
	Color( int32_t r, int32_t g, int32_t b, int32_t a = 255 );
	Color( std::string & str );

	Color operator*( float_t v ) const;
	Color operator*=( float_t v );
	Color operator-( Color v ) const;
	Color operator-=( Color v );
	Color operator+( Color v ) const;
	Color operator+=( Color v );

	static void scale( Color & color, float_t s );

	static Color make( const Color & clr, float_t alpha = 1.f );
	static Color make( e_pal pal_idx, float_t alpha = 1.f );
};

}
