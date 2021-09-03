
namespace war
{

struct Vec3
{
	float x, y, z;

	static const Vec3 zero;

	Vec3();
	Vec3( int32_t x, int32_t y, int32_t z );
	Vec3( float x, float y, float z );
	Vec3( std::string_view str );
};

}
