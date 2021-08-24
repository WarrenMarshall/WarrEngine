
namespace war
{

struct vec3 final
{
	float x, y, z;

	static const vec3 zero;

	vec3();
	vec3( int x, int y, int z );
	vec3( float x, float y, float z );
	vec3( std::string_view str );
};

}
