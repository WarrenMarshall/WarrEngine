
namespace war
{

struct Vec3 final
{
	f32 x, y, z;

	static const Vec3 zero;

	Vec3();
	Vec3( i32 x, i32 y, i32 z );
	Vec3( f32 x, f32 y, f32 z );
	Vec3( std::string_view str );
};

}
