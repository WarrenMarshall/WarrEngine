
namespace war
{

struct Vec3 final
{
	float x, y, z;

	static const Vec3 zero;

	Vec3();
	Vec3( int x, int y, int z );
	Vec3( float x, float y, float z );
	Vec3( std::string_view str );
};

}
