
namespace war
{

struct Render_Vertex final
{
	Render_Vertex() = default;
	Render_Vertex( const Vec2& pos, const Vec2& uv, const Color& color, float glow );
	Render_Vertex( const Vec2& pos );
	Render_Vertex( const Vec3& pos, const Vec2& uv, const Color& color, float glow );
	Render_Vertex( const Vec3& pos );
	Render_Vertex( const Render_Vertex& other );

	float x = 0.f, y = 0.f, z = 0.f;
	float u = 0.f, v = 0.f;
	float r = 1.f, g = 1.f, b = 1.f, a = 1.f;
	float glow = 0.f;
	size_t texture_id = 0;
	int pick_id = 1;
};

// ----------------------------------------------------------------------------

struct Render_Triangle final
{
	std::array<Render_Vertex,3> verts;
	Render_Triangle( const Render_Vertex& v0, const Render_Vertex& v1, const Render_Vertex& v2, const Color& color, float glow );
};

// ----------------------------------------------------------------------------

struct Render_Line final
{
	std::array<Render_Vertex, 2> verts;
	Render_Line( const Render_Vertex& v0, const Render_Vertex& v1, const Color& color, float glow );
};

}
