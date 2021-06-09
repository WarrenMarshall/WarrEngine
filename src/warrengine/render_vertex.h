
namespace war
{

struct render_vertex
{
	render_vertex() = default;
	render_vertex( const vec2& pos, const vec2& uv, const color& color, float glow );
	render_vertex( const vec3& pos, const vec2& uv, const color& color, float glow );
	render_vertex( const render_vertex& other );

	float x = 0.f, y = 0.f, z = 0.f;
	float u = 0.f, v = 0.f;
	float r = 1.f, g = 1.f, b = 1.f, a = 1.f;
	float glow = 0.f;
	int texture_id = -1;
	int pick_id = 1;
};

}
