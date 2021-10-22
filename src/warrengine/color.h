
namespace war
{

struct Color final
{
	f32 r = 0.f, g = 0.f, b = 0.f, a = 1.f;

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
	Color( f32 r, f32 g, f32 b, f32 a = 1.f );
	Color( f32 v, f32 a = 1.f );
	Color( i32 r, i32 g, i32 b, i32 a = 255 );
	Color( std::string & str );

	Color operator*( f32 v ) const;
	Color operator*=( f32 v );
	Color operator-( Color v ) const;
	Color operator-=( Color v );
	Color operator+( Color v ) const;
	Color operator+=( Color v );

	static void scale( Color & color, f32 s );

	static Color make( const Color & clr, f32 alpha = 1.f );
	static Color make( e_pal pal_idx, f32 alpha = 1.f );
};

}
